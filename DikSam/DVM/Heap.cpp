#include "StdAfx.h"
#include "Heap.h"
#include "Debug.h"
#include "Memory.h"
#include "Util.h"
#include "DVM_pri.h"

#ifdef DBG_assert
#undef DBG_assert
#endif
#define DBG_assert(expression, arg) ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

#ifdef MEM_malloc
#undef MEM_malloc
#endif
#define MEM_malloc(size)                    (m_Memory.Malloc(__FILE__, __LINE__, size))

#ifdef MEM_free
#undef MEM_free
#endif
#define MEM_free(ptr)                       (m_Memory.Free(ptr))

CHeap::CHeap(Debug& debug, Memory& memory, Util& util)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Util(util)
{

}

CHeap::~CHeap()
{

}

void CHeap::CheckGC(DVM_VirtualMachine *pVirtualMachine)
{
    if (pVirtualMachine->heap.current_heap_size > pVirtualMachine->heap.current_threshold)
    {
        GarbageCollect(pVirtualMachine);
        pVirtualMachine->heap.current_threshold = pVirtualMachine->heap.current_heap_size + HEAP_THRESHOLD_SIZE;
    }
}

void CHeap::GarbageCollect(DVM_VirtualMachine *pVirtualMachine)
{
    bool bCallFinalizer;

    do 
    {
        MarkObjects(pVirtualMachine);
        bCallFinalizer = SweepObjects(pVirtualMachine);
    } while (bCallFinalizer);
}

DVM_ObjectRef CHeap::LiteralToString(DVM_VirtualMachine *pVirtualMachine, DVM_Char *str)
{
    DVM_ObjectRef obj = AllocObject(pVirtualMachine, STRING_OBJECT);

    obj.v_table = pVirtualMachine->string_v_table;
    obj.data->u.string.string = str;
    obj.data->u.string.length = std::basic_string<DVM_Char>(str).length();
    obj.data->u.string.is_literal = DVM_TRUE;

    return obj;
}

DVM_ObjectRef CHeap::CreateString(DVM_VirtualMachine *pVirtualMachine, DVM_Char *str)
{
    DVM_ObjectRef obj = AllocObject(pVirtualMachine, STRING_OBJECT);

    obj.v_table = pVirtualMachine->string_v_table;
    obj.data->u.string.string = str;

    int iLen = nullptr == str ? 0 : std::basic_string<DVM_Char>(str).length();
    pVirtualMachine->heap.current_heap_size += sizeof(DVM_Char) * (iLen + 1);
    
    obj.data->u.string.is_literal = DVM_FALSE;
    obj.data->u.string.length = iLen;

    return obj;
}

DVM_ObjectRef CHeap::CreateArrayInt(DVM_VirtualMachine *pVirtualMachine, int iSize)
{
    DVM_ObjectRef object = AllocArray(pVirtualMachine, INT_ARRAY, iSize);

    object.data->u.array.u.int_array = (int *)MEM_malloc(sizeof(int) * iSize);
    pVirtualMachine->heap.current_heap_size += sizeof(int) * iSize;

    for (int i = 0; i < iSize; i++)
    {
        object.data->u.array.u.int_array[i] = 0;
    }

    return object;
}

DVM_ObjectRef CHeap::CreateArrayDouble(DVM_VirtualMachine *pVirtualMachine, int iSize)
{
    DVM_ObjectRef object = AllocArray(pVirtualMachine, DOUBLE_ARRAY, iSize);

    object.data->u.array.u.double_array = (double *)MEM_malloc(sizeof(double) * iSize);
    pVirtualMachine->heap.current_heap_size += sizeof(double) * iSize;

    for (int i = 0; i < iSize; i++)
    {
        object.data->u.array.u.double_array[i] = 0.0;
    }

    return object;
}

DVM_ObjectRef CHeap::CreateArrayObject(DVM_VirtualMachine *pVirtualMachine, int iSize)
{
    DVM_ObjectRef object = AllocArray(pVirtualMachine, OBJECT_ARRAY, iSize);

    object.data->u.array.u.object = (DVM_ObjectRef*)MEM_malloc(sizeof(DVM_ObjectRef) * iSize);
    pVirtualMachine->heap.current_heap_size += sizeof(DVM_ObjectRef) * iSize;

    for (int i = 0; i < iSize; i++)
    {
        object.data->u.array.u.object[i] = { nullptr, nullptr };
    }

    return object;
}

DVM_ObjectRef CHeap::CreateClassObject(DVM_VirtualMachine *pVirtualMachine, int iIndex)
{
    DVM_ObjectRef obj = AllocObject(pVirtualMachine, CLASS_OBJECT);
    ExecClass *pExecClass = pVirtualMachine->classes[iIndex];

    obj.v_table = pExecClass->class_table;
    obj.data->u.class_object.field_count = pExecClass->field_count;
    obj.data->u.class_object.field = (DVM_Value*)MEM_malloc(sizeof(DVM_Value) * pExecClass->field_count);

    for (int i = 0; i < pExecClass->field_count; i++)
    {
        m_Util.InitializeValue(pExecClass->field_type[i], &obj.data->u.class_object.field[i]);
    }

    return obj;
}

bool CHeap::IsReferenceType(DVM_TypeSpecifier *pType)
{
    if ((DVM_STRING_TYPE == pType->basic_type || DVM_CLASS_TYPE == pType->basic_type) && 0 == pType->derive_count
        || pType->derive_count > 0 && DVM_ARRAY_DERIVE == pType->derive[0].tag)
        return true;

    return false;
}

DVM_ObjectRef CHeap::AllocObject(DVM_VirtualMachine *pVirtualMachine, ObjectType enType)
{
    DVM_ObjectRef obj;

    CheckGC(pVirtualMachine);

    obj.v_table = nullptr;
    obj.data = (DVM_Object*)MEM_malloc(sizeof(DVM_Object));

    pVirtualMachine->heap.current_heap_size += sizeof(DVM_Object);
    
    obj.data->type = enType;
    obj.data->marked = DVM_FALSE;
    
    obj.data->prev = nullptr;
    obj.data->next = pVirtualMachine->heap.header;
    pVirtualMachine->heap.header = obj.data;

    if (obj.data->next)
    {
        obj.data->next->prev = obj.data;
    }

    return obj;
}

DVM_ObjectRef CHeap::AllocArray(DVM_VirtualMachine *pVirtualMachine, ArrayType enType, int iSize)
{
    DVM_ObjectRef object = AllocObject(pVirtualMachine, ARRAY_OBJECT);

    object.data->u.array.type = enType;
    object.data->u.array.size = iSize;
    object.data->u.array.alloc_size = iSize;
    object.v_table = pVirtualMachine->array_v_table;

    return object;
}

void CHeap::MarkObjects(DVM_VirtualMachine *pVirtualMachine)
{
    for (DVM_Object *pObj = pVirtualMachine->heap.header; pObj; pObj = pObj->next)
    {
        ResetMark(pObj);
    }

    for (ExecutableEntry *pos = pVirtualMachine->executable_entry; pos; pos = pos->next)
    {
        for (int i = 0; i < pos->static_v.variable_count; i++)
        {
            if (IsReferenceType(pos->executable->global_variable[i].type))
            {
                Mark(&pos->static_v.variable[i].object);
            }
        }
    }

    for (int i = 0; i < pVirtualMachine->stack.stack_pointer; i++)
    {
        if (DVM_TRUE == pVirtualMachine->stack.pointer_flags[i])
        {
            Mark(&pVirtualMachine->stack.stack[i].object);
        }
    }

    Mark(&pVirtualMachine->current_exception);
}

bool CHeap::SweepObjects(DVM_VirtualMachine *pVirtualMachine)
{
    bool bCallFinalizer = false;

    for (DVM_Object *pObj = pVirtualMachine->heap.header; pObj;)
    {
        if (DVM_FALSE == pObj->marked)
        {
            if (pObj->prev)
            {
                pObj->prev->next = pObj->next;
            }
            else
            {
                pVirtualMachine->heap.header = pObj->next;
            }

            if (pObj->next)
            {
                pObj->next->prev = pObj->prev;
            }

            DVM_Object *tmp = pObj->next;
            if (DisposeObject(pVirtualMachine, pObj))
            {
                bCallFinalizer = true;
            }
            pObj = tmp;
        }
        else
        {
            pObj = pObj->next;
        }
    }

    return bCallFinalizer;
}

void CHeap::ResetMark(DVM_Object *pObj)
{
    pObj->marked = DVM_FALSE;
}

void CHeap::Mark(DVM_ObjectRef *pRef)
{
    if (nullptr == pRef->data)
        return;

    if (DVM_TRUE == pRef->data->marked)
        return;

    pRef->data->marked = DVM_TRUE;

    if (ARRAY_OBJECT == pRef->data->type && OBJECT_ARRAY == pRef->data->u.array.type)
    {
        for (int i = 0; i < pRef->data->u.array.size; i++)
        {
            Mark(&pRef->data->u.array.u.object[i]);
        }
    }
    else if (CLASS_OBJECT == pRef->data->type)
    {
        ExecClass *pEC = pRef->v_table->exec_class;

        for (int i = 0; i < pEC->field_count; i++)
        {
            if (IsReferenceType(pEC->field_type[i]))
            {
                Mark(&pRef->data->u.class_object.field[i].object);
            }
        }
    }
}

bool CHeap::DisposeObject(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pObj)
{
    bool bCallFinalizer = false;

    switch (pObj->type)
    {
    case STRING_OBJECT :
        if (DVM_FALSE == pObj->u.string.is_literal)
        {
            int iLen = pObj->u.string.string == nullptr ? 0 : std::basic_string<DVM_Char>(pObj->u.string.string).length();
            pVirtualMachine->heap.current_heap_size -= sizeof(DVM_Char) * (iLen + 1);
            MEM_free(pObj->u.string.string);
        }
        break;

    case ARRAY_OBJECT :
        switch (pObj->u.array.type)
        {
        case INT_ARRAY :
            pVirtualMachine->heap.current_heap_size -= sizeof(int) * pObj->u.array.alloc_size;
            MEM_free(pObj->u.array.u.int_array);
            break;

        case DOUBLE_ARRAY :
            pVirtualMachine->heap.current_heap_size -= sizeof(double) * pObj->u.array.alloc_size;
            MEM_free(pObj->u.array.u.double_array);
            break;

        case OBJECT_ARRAY :
            pVirtualMachine->heap.current_heap_size -= sizeof(DVM_Object*) * pObj->u.array.alloc_size;
            MEM_free(pObj->u.array.u.object);
            break;

        case FUNCTION_INDEX_ARRAY :
            pVirtualMachine->heap.current_heap_size -= sizeof(int)* pObj->u.array.alloc_size;
            MEM_free(pObj->u.array.u.function_index);
            break;

        default :
            DBG_assert(0, ("array.type..", pObj->u.array.type));
        }
        break;

    case CLASS_OBJECT :
        pVirtualMachine->heap.current_heap_size -= sizeof(DVM_Value)* pObj->u.class_object.field_count;
        MEM_free(pObj->u.class_object.field);
        break;

    default :
        DBG_assert(0, ("bad type..", pObj->type));
    }

    pVirtualMachine->heap.current_heap_size -= sizeof(DVM_Object);
    MEM_free(pObj);

    return bCallFinalizer;
}