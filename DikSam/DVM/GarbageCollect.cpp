#include "StdAfx.h"
#include "GarbageCollect.h"
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

GarbageCollect::GarbageCollect(Debug& debug, Memory& memory, Util& util)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Util(util)
{

}

GarbageCollect::~GarbageCollect()
{

}

void GarbageCollect::GC(DVM_VirtualMachine *pVirtualMachine)
{
    MarkObjects(pVirtualMachine);
    SweepObjects(pVirtualMachine);
}

DVM_ObjectRef GarbageCollect::LiteralToStringI(DVM_VirtualMachine *pVirtualMachine, DVM_Char *str)
{
    DVM_ObjectRef obj = AllocObject(pVirtualMachine, STRING_OBJECT);

    obj.v_table = pVirtualMachine->string_v_table;
    obj.data->u.string.string = str;
    obj.data->u.string.length = std::basic_string<DVM_Char>(str).length();
    obj.data->u.string.is_literal = DVM_TRUE;

    return obj;
}

DVM_ObjectRef GarbageCollect::CreateString(DVM_VirtualMachine *pVirtualMachine, DVM_Char *str)
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

DVM_ObjectRef GarbageCollect::CreateArrayIntI(DVM_VirtualMachine *pVirtualMachine, int iSize)
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

DVM_Object* GarbageCollect::CreateArrayInt(DVM_VirtualMachine *pVirtualMachine, int iSize)
{
    DVM_Object *pObject = CreateArrayIntI(pVirtualMachine, iSize);

    for (int i = 0; i < iSize; i++)
    {
        pObject->u.array.u.int_array[i] = 0;
    }

    return pObject;
}

DVM_ObjectRef GarbageCollect::CreateArrayDoubleI(DVM_VirtualMachine *pVirtualMachine, int iSize)
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

DVM_Object* GarbageCollect::CreateArrayDouble(DVM_VirtualMachine *pVirtualMachine, int iSize)
{
    DVM_Object *pObject = CreateArrayDoubleI(pVirtualMachine, iSize);

    for (int i = 0; i < iSize; i++)
    {
        pObject->u.array.u.double_array[i] = 0.0;
    }

    return pObject;
}

DVM_ObjectRef GarbageCollect::CreateArrayObjectI(DVM_VirtualMachine *pVirtualMachine, int iSize)
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

DVM_Object* GarbageCollect::CreateArrayObject(DVM_VirtualMachine *pVirtualMachine, int iSize)
{
    DVM_Object *pObject = CreateArrayObjectI(pVirtualMachine, iSize);

    for (int i = 0; i < iSize; i++)
    {
        pObject->u.array.u.object[i] = nullptr;
    }

    return pObject;
}

DVM_ObjectRef GarbageCollect::CreateClassObjectI(DVM_VirtualMachine *pVirtualMachine, int iIndex)
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

void GarbageCollect::CheckGC(DVM_VirtualMachine *pVirtualMachine)
{
    if (pVirtualMachine->heap.current_heap_size > pVirtualMachine->heap.current_threshold)
    {
        GC(pVirtualMachine);
        pVirtualMachine->heap.current_threshold = pVirtualMachine->heap.current_heap_size + HEAP_THRESHOLD_SIZE;
    }
}

DVM_ObjectRef GarbageCollect::AllocObject(DVM_VirtualMachine *pVirtualMachine, ObjectType enType)
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

DVM_ObjectRef GarbageCollect::AllocArray(DVM_VirtualMachine *pVirtualMachine, ArrayType enType, int iSize)
{
    DVM_ObjectRef object = AllocObject(pVirtualMachine, ARRAY_OBJECT);

    object.data->u.array.type = enType;
    object.data->u.array.size = iSize;
    object.data->u.array.alloc_size = iSize;
    object.v_table = pVirtualMachine->array_v_table;

    return object;
}

void GarbageCollect::MarkObjects(DVM_VirtualMachine *pVirtualMachine)
{
    for (DVM_Object *pObj = pVirtualMachine->heap.header; pObj; pObj = pObj->next)
    {
        ResetMark(pObj);
    }

    for (int i = 0; i < pVirtualMachine->static_v.variable_count; i++)
    {
        if (DVM_STRING_TYPE == pVirtualMachine->executable->global_variable[i].type->basic_type
            || (pVirtualMachine->executable->global_variable[i].type->derive != nullptr
                && DVM_ARRAY_DERIVE == pVirtualMachine->executable->global_variable[i].type->derive[0].tag))
        {
            Mark(pVirtualMachine->static_v.variable[i].object);
        }
    }

    for (int i = 0; i < pVirtualMachine->stack.stack_pointer; i++)
    {
        if (DVM_TRUE == pVirtualMachine->stack.pointer_flags[i])
        {
            Mark(pVirtualMachine->stack.stack[i].object);
        }
    }
}

void GarbageCollect::SweepObjects(DVM_VirtualMachine *pVirtualMachine)
{
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
            DisposeObject(pVirtualMachine, pObj);
            pObj = tmp;
        }
        else
        {
            pObj = pObj->next;
        }
    }
}

void GarbageCollect::ResetMark(DVM_Object *pObj)
{
    pObj->marked = DVM_FALSE;
}

void GarbageCollect::Mark(DVM_Object *pObj)
{
    if (nullptr == pObj)
        return;

    if (pObj->marked)
        return;

    pObj->marked = DVM_TRUE;

    if (ARRAY_OBJECT == pObj->type && OBJECT_ARRAY == pObj->u.array.type)
    {
        for (int i = 0; i < pObj->u.array.size; i++)
        {
            Mark(pObj->u.array.u.object[i]);
        }
    }
}

void GarbageCollect::DisposeObject(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pObj)
{
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

        default :
            DBG_assert(0, ("array.type..", pObj->u.array.type));
        }
        break;

    default :
        DBG_assert(0, ("bad type..", pObj->type));
    }

    pVirtualMachine->heap.current_heap_size -= sizeof(DVM_Object);
    MEM_free(pObj);
}