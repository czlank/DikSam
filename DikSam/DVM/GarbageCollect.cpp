#include "StdAfx.h"
#include "GarbageCollect.h"
#include "Debug.h"
#include "Memory.h"
#include "DVM_pri.h"

GarbageCollect::GarbageCollect(Debug& debug, Memory& memory)
    : m_Debug(debug)
    , m_Memory(memory)
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

DVM_Object* GarbageCollect::LiteralToString(DVM_VirtualMachine *pVirtualMachine, DVM_Char *str)
{
    DVM_Object *pObj = AllocObject(pVirtualMachine, STRING_OBJECT);

    pObj->u.string.string = str;
    pObj->u.string.is_literal = DVM_TRUE;

    return pObj;
}

DVM_Object* GarbageCollect::CreateString(DVM_VirtualMachine *pVirtualMachine, DVM_Char *str)
{
    DVM_Object *pObj = AllocObject(pVirtualMachine, STRING_OBJECT);

    pObj->u.string.string = str;

    int iLen = str == nullptr ? 0 : std::basic_string<DVM_Char>(str).length();
    pVirtualMachine->heap.current_heap_size += sizeof(DVM_Char) * (iLen + 1);
    pObj->u.string.is_literal = DVM_FALSE;

    return pObj;
}

DVM_Object* GarbageCollect::CreateArrayIntI(DVM_VirtualMachine *pVirtualMachine, int iSize)
{
    DVM_Object *pObject = AllocArray(pVirtualMachine, INT_ARRAY, iSize);

    pObject->u.array.u.int_array = (int *)GARBAGECOLLECT_MEM_Malloc(sizeof(int) * iSize);
    pVirtualMachine->heap.current_heap_size += sizeof(int) * iSize;

    return pObject;
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

DVM_Object* GarbageCollect::CreateArrayDoubleI(DVM_VirtualMachine *pVirtualMachine, int iSize)
{
    DVM_Object *pObject = AllocArray(pVirtualMachine, DOUBLE_ARRAY, iSize);

    pObject->u.array.u.double_array = (double *)GARBAGECOLLECT_MEM_Malloc(sizeof(double) * iSize);
    pVirtualMachine->heap.current_heap_size += sizeof(double) * iSize;

    return pObject;
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

DVM_Object* GarbageCollect::CreateArrayObjectI(DVM_VirtualMachine *pVirtualMachine, int iSize)
{
    DVM_Object *pObject = AllocArray(pVirtualMachine, OBJECT_ARRAY, iSize);

    pObject->u.array.u.object = (DVM_Object **)GARBAGECOLLECT_MEM_Malloc(sizeof(DVM_Object*) * iSize);
    pVirtualMachine->heap.current_heap_size += sizeof(DVM_Object*) * iSize;

    return pObject;
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

void GarbageCollect::CheckGC(DVM_VirtualMachine *pVirtualMachine)
{
    if (pVirtualMachine->heap.current_heap_size > pVirtualMachine->heap.current_threshold)
    {
        GC(pVirtualMachine);
        pVirtualMachine->heap.current_threshold = pVirtualMachine->heap.current_heap_size + HEAP_THRESHOLD_SIZE;
    }
}

DVM_Object* GarbageCollect::AllocObject(DVM_VirtualMachine *pVirtualMachine, ObjectType enType)
{
    CheckGC(pVirtualMachine);

    DVM_Object *pObj = (DVM_Object*)GARBAGECOLLECT_MEM_Malloc(sizeof(DVM_Object));

    pVirtualMachine->heap.current_heap_size += sizeof(DVM_Object);
    
    pObj->type = enType;
    pObj->marked = DVM_FALSE;
    
    pObj->prev = nullptr;
    pObj->next = pVirtualMachine->heap.header;
    pVirtualMachine->heap.header = pObj;
    if (pObj->next)
    {
        pObj->next->prev = pObj;
    }

    return pObj;
}

DVM_Object* GarbageCollect::AllocArray(DVM_VirtualMachine *pVirtualMachine, ArrayType enType, int iSize)
{
    DVM_Object *pObject = AllocObject(pVirtualMachine, ARRAY_OBJECT);

    pObject->u.array.type = enType;
    pObject->u.array.size = iSize;
    pObject->u.array.alloc_size = iSize;

    return pObject;
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
            GARBAGECOLLECT_MEM_Free(pObj->u.string.string);
        }
        break;

    case ARRAY_OBJECT :
        switch (pObj->u.array.type)
        {
        case INT_ARRAY :
            pVirtualMachine->heap.current_heap_size -= sizeof(int) * pObj->u.array.alloc_size;
            GARBAGECOLLECT_MEM_Free(pObj->u.array.u.int_array);
            break;

        case DOUBLE_ARRAY :
            pVirtualMachine->heap.current_heap_size -= sizeof(double) * pObj->u.array.alloc_size;
            GARBAGECOLLECT_MEM_Free(pObj->u.array.u.double_array);
            break;

        case OBJECT_ARRAY :
            pVirtualMachine->heap.current_heap_size -= sizeof(DVM_Object*) * pObj->u.array.alloc_size;
            GARBAGECOLLECT_MEM_Free(pObj->u.array.u.object);
            break;

        default :
            GARBAGECOLLECT_DBG_Assert(0, ("array.type..", pObj->u.array.type));
        }
        break;

    default :
        GARBAGECOLLECT_DBG_Assert(0, ("bad type..", pObj->type));
    }

    pVirtualMachine->heap.current_heap_size -= sizeof(DVM_Object);
    GARBAGECOLLECT_MEM_Free(pObj);
}