#include "StdAfx.h"
#include "Native.h"
#include "DVM_pri.h"
#include "Debug.h"
#include "Memory.h"
#include "Error.h"
#include "Heap.h"
#include "Load.h"

#ifdef DBG_assert
#undef DBG_assert
#endif
#define DBG_assert(expression, arg) ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

#ifdef DBG_panic
#undef DBG_panic
#endif
#define DBG_panic(arg)                      (m_Debug.Panic(__FILE__, __LINE__, arg))

#ifdef MEM_malloc
#undef MEM_malloc
#endif
#define MEM_malloc(size)                    (m_Memory.Malloc(__FILE__, __LINE__, size))

#ifdef MEM_realloc
#undef MEM_realloc
#endif
#define MEM_realloc(ptr, size)              (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))

#ifdef MEM_strdup
#undef MEM_strdup
#endif
#define MEM_strdup(ptr)                     (m_Memory.StrDUP(__FILE__, __LINE__, ptr))

Native::Native(Debug& debug, Memory& memory, Error& error, CHeap& heap, Load& load)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Error(error)
    , m_Heap(heap)
    , m_Load(load)
{

}

Native::~Native()
{

}

void Native::AddNativeFunctions(DVM_VirtualMachine *pVirtualMachine)
{
    m_Load.AddNativeFunction(pVirtualMachine, "diksam.lang", "print", &Native::PrintProc, 1, DVM_FALSE, DVM_FALSE);
    m_Load.AddNativeFunction(pVirtualMachine, "diksam.lang", "sleep", &Native::SleepProc, 1, DVM_FALSE, DVM_FALSE);

    m_Load.AddNativeFunction(pVirtualMachine, BUILT_IN_METHOD_PACKAGE_NAME, ARRAY_PREFIX ARRAY_METHOD_SIZE, &Native::ArraySizeProc, 0, DVM_TRUE, DVM_FALSE);
    m_Load.AddNativeFunction(pVirtualMachine, BUILT_IN_METHOD_PACKAGE_NAME, ARRAY_PREFIX ARRAY_METHOD_RESIZE, &Native::ArrayResizeProc, 1, DVM_TRUE, DVM_FALSE);
    m_Load.AddNativeFunction(pVirtualMachine, BUILT_IN_METHOD_PACKAGE_NAME, ARRAY_PREFIX ARRAY_METHOD_INSERT, &Native::ArrayInsertProc, 2, DVM_TRUE, DVM_FALSE);
    m_Load.AddNativeFunction(pVirtualMachine, BUILT_IN_METHOD_PACKAGE_NAME, ARRAY_PREFIX ARRAY_METHOD_REMOVE, &Native::ArrayRemoveProc, 1, DVM_TRUE, DVM_FALSE);
    m_Load.AddNativeFunction(pVirtualMachine, BUILT_IN_METHOD_PACKAGE_NAME, ARRAY_PREFIX ARRAY_METHOD_ADD,    &Native::ArrayAddProc,    1, DVM_TRUE, DVM_FALSE);

    m_Load.AddNativeFunction(pVirtualMachine, BUILT_IN_METHOD_PACKAGE_NAME, STRING_PREFIX STRING_METHOD_LENGTH, &Native::StringLengthProc, 0, DVM_TRUE, DVM_FALSE);
    m_Load.AddNativeFunction(pVirtualMachine, BUILT_IN_METHOD_PACKAGE_NAME, STRING_PREFIX STRING_METHOD_SUBSTR, &Native::StringSubStrProc, 2, DVM_TRUE, DVM_FALSE);
}

DVM_Value Native::PrintProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    return pThis->Print(pVirtualMachine, iArgCount, pArgs);
}

DVM_Value Native::SleepProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    return pThis->Sleep(pVirtualMachine, iArgCount, pArgs);
}

DVM_Value Native::ArraySizeProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    return pThis->ArraySizeProc(pVirtualMachine, iArgCount, pArgs);
}

DVM_Value Native::ArrayResizeProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    return pThis->ArrayResizeProc(pVirtualMachine, iArgCount, pArgs);
}

DVM_Value Native::ArrayInsertProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    return pThis->ArrayInsertProc(pVirtualMachine, iArgCount, pArgs);
}

DVM_Value Native::ArrayRemoveProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    return pThis->ArrayRemoveProc(pVirtualMachine, iArgCount, pArgs);
}

DVM_Value Native::ArrayAddProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    return pThis->ArrayAddProc(pVirtualMachine, iArgCount, pArgs);
}

DVM_Value Native::StringLengthProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    return pThis->StringLengthProc(pVirtualMachine, iArgCount, pArgs);
}

DVM_Value Native::StringSubStrProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    return pThis->StringSubStrProc(pVirtualMachine, iArgCount, pArgs);
}

DVM_Value Native::Print(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    DBG_assert(1 == iArgCount, ("iArgCount..", iArgCount));

    if (pArgs[0].object.data)
    {
        std::wcout << pArgs[0].object.data->u.string.string << std::endl;
    }

    DVM_Value ret;
    ret.int_value = 0;

    return ret;
}

DVM_Value Native::Sleep(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    DBG_assert(1 == iArgCount, ("iArgCount..", iArgCount));

    ::Sleep(pArgs[0].int_value);

    DVM_Value ret;
    ret.int_value = 0;

    return ret;
}

DVM_Value Native::ArraySizeProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    DBG_assert(0 == iArgCount, ("iArgCount..", iArgCount));

    DVM_Object *pArray = pArgs[0].object.data;
    DBG_assert(ARRAY_OBJECT == pArray->type, ("pArray->type..", pArray->type));

    DVM_Value ret;
    ret.int_value = ArraySize(pVirtualMachine, pArray);

    return ret;
}

DVM_Value Native::ArrayResizeProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    DBG_assert(1 == iArgCount, ("iArgCount..", iArgCount));

    int iNewSize = pArgs[0].int_value;
    DVM_Object *pArray = pArgs[1].object.data;
    DBG_assert(ARRAY_OBJECT == pArray->type, ("pArray->type..", pArray->type));
    
    ArrayResize(pVirtualMachine, pArray, iNewSize);

    DVM_Value ret;
    ret.int_value = 0;

    return ret;
}

DVM_Value Native::ArrayInsertProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    DBG_assert(2 == iArgCount, ("iArgCount..", iArgCount));

    int iPos = pArgs[0].int_value;
    DVM_Value value = pArgs[1];
    DVM_Object *pArray = pArgs[2].object.data;
    DBG_assert(ARRAY_OBJECT == pArray->type, ("pArray->type..", pArray->type));

    int iArraySize = ArraySize(pVirtualMachine, pArray);

    DVM_Value ret;
    ret.int_value = 0;

    if (iPos < 0 || iPos > iArraySize)
    {
        m_Error.DVMError(pVirtualMachine->current_executable->executable, pVirtualMachine->current_function, pVirtualMachine->pc,
            INSERT_INDEX_OUT_OF_BOUNDS_ERR,
            INT_MESSAGE_ARGUMENT, "pos", iPos, INT_MESSAGE_ARGUMENT, "size", iArraySize,
            MESSAGE_ARGUMENT_END);

        return ret;
    }

    ArrayInsert(pVirtualMachine, pArray, iPos, value);

    return ret;
}

DVM_Value Native::ArrayRemoveProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    DBG_assert(1 == iArgCount, ("iArgCount..", iArgCount));

    int iPos = pArgs[0].int_value;
    DVM_Object *pArray = pArgs[1].object.data;
    DBG_assert(ARRAY_OBJECT == pArray->type, ("pArray->type..", pArray->type));

    int iArraySize = ArraySize(pVirtualMachine, pArray);

    DVM_Value ret;
    ret.int_value = 0;

    if (iPos < 0 || iPos >= iArraySize)
    {
        m_Error.DVMError(pVirtualMachine->current_executable->executable, pVirtualMachine->current_function, pVirtualMachine->pc,
            REMOVE_INDEX_OUT_OF_BOUNDS_ERR,
            INT_MESSAGE_ARGUMENT, "pos", iPos, INT_MESSAGE_ARGUMENT, "size", iArraySize,
            MESSAGE_ARGUMENT_END);

        return ret;
    }

    ArrayRemove(pVirtualMachine, pArray, iPos);

    return ret;
}

DVM_Value Native::ArrayAddProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    DBG_assert(1 == iArgCount, ("iArgCount..", iArgCount));

    DVM_Value value = pArgs[0];
    DVM_Object *pArray = pArgs[1].object.data;
    DBG_assert(ARRAY_OBJECT == pArray->type, ("pArray->type..", pArray->type));
    int iArraySize = ArraySize(pVirtualMachine, pArray);

    ArrayInsert(pVirtualMachine, pArray, iArraySize, value);

    DVM_Value ret;
    ret.int_value = 0;

    return ret;
}

DVM_Value Native::StringLengthProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    DBG_assert(0 == iArgCount, ("iArgCount..", iArgCount));

    DVM_Object *pStr = pArgs[0].object.data;
    DBG_assert(STRING_OBJECT == pStr->type, ("pStr->type..", pStr->type));

    DVM_Value ret;
    ret.int_value = pStr->u.string.string ? std::basic_string<DVM_Char>(pStr->u.string.string).length() : 0;

    return ret;
}

DVM_Value Native::StringSubStrProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    DBG_assert(2 == iArgCount, ("iArgCount..", iArgCount));

    int iPos = pArgs[0].int_value;
    int iLen = pArgs[1].int_value;
    DVM_Object *pStr = pArgs[2].object.data;

    DBG_assert(STRING_OBJECT == pStr->type, ("pStr->type..", pStr->type));
    int iOrLen = pStr->u.string.string ? std::basic_string<DVM_Char>(pStr->u.string.string).length() : 0;

    DVM_Value ret;
    ret.int_value = 0;

    if (iPos < 0 || iPos >= iOrLen)
    {
        m_Error.DVMError(pVirtualMachine->current_executable->executable, pVirtualMachine->current_function, pVirtualMachine->pc,
            STRING_POS_OUT_OF_BOUNDS_ERR,
            INT_MESSAGE_ARGUMENT, "len", iOrLen, INT_MESSAGE_ARGUMENT, "pos", iPos,
            MESSAGE_ARGUMENT_END);

        return ret;
    }

    if (iLen < 0 || iPos + iLen > iOrLen)
    {
        m_Error.DVMError(pVirtualMachine->current_executable->executable, pVirtualMachine->current_function, pVirtualMachine->pc,
            STRING_SUBSTR_LEN_ERR,
            INT_MESSAGE_ARGUMENT, "len", iLen,
            MESSAGE_ARGUMENT_END);

        return ret;
    }

    DVM_Char *pNewStr = (DVM_Char*)MEM_malloc(sizeof(DVM_Char) * iLen);
    for (int i = 0; i < iLen; i++)
    {
        pNewStr[i] = pStr->u.string.string[iPos + i];
    }
    pNewStr[iLen] = '\0';

    ret.object = m_Heap.CreateString(pVirtualMachine, pNewStr);

    return ret;
}

int Native::ArraySize(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pArray)
{
    return pArray->u.array.size;
}

void Native::ArrayResize(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pArray, int iNewSize)
{
    DBG_assert(ARRAY_OBJECT == pArray->type, ("pArray->type..", pArray->type));

    bool bNeedRealloc = false;
    int iNewAllocSize;

    if (iNewSize > pArray->u.array.alloc_size)
    {
        iNewAllocSize = pArray->u.array.alloc_size * 2;

        if (iNewAllocSize < iNewSize)
        {
            iNewAllocSize = iNewSize + ARRAY_ALLOC_SIZE;
        }
        else if (iNewAllocSize - pArray->u.array.alloc_size > ARRAY_ALLOC_SIZE)
        {
            iNewAllocSize = pArray->u.array.alloc_size + ARRAY_ALLOC_SIZE;
        }

        bNeedRealloc = true;
    }
    else if (pArray->u.array.alloc_size - iNewSize > ARRAY_ALLOC_SIZE)
    {
        iNewAllocSize = iNewSize;
        bNeedRealloc = true;
    }

    if (bNeedRealloc)
    {
        m_Heap.CheckGC(pVirtualMachine);

        switch (pArray->u.array.type)
        {
        case INT_ARRAY :
            pArray->u.array.u.int_array = (int*)MEM_realloc(pArray->u.array.u.int_array, sizeof(int) * iNewAllocSize);
            pVirtualMachine->heap.current_heap_size += sizeof(int) * (iNewAllocSize - pArray->u.array.alloc_size);

            for (int i = pArray->u.array.size; i < iNewSize; i++)
            {
                pArray->u.array.u.int_array[i] = 0;
            }

            break;

        case DOUBLE_ARRAY :
            pArray->u.array.u.double_array = (double*)MEM_realloc(pArray->u.array.u.double_array, sizeof(double) * iNewAllocSize);
            pVirtualMachine->heap.current_heap_size += sizeof(double) * (iNewAllocSize - pArray->u.array.alloc_size);

            for (int i = pArray->u.array.size; i < iNewSize; i++)
            {
                pArray->u.array.u.double_array[i] = 0.0;
            }

            break;

        case OBJECT_ARRAY :
            pArray->u.array.u.object = (DVM_ObjectRef*)MEM_realloc(pArray->u.array.u.object, sizeof(DVM_ObjectRef) * iNewAllocSize);
            pVirtualMachine->heap.current_heap_size += sizeof(DVM_ObjectRef) * (iNewAllocSize - pArray->u.array.alloc_size);

            for (int i = pArray->u.array.size; i < iNewSize; i++)
            {
                pArray->u.array.u.object[i] = { nullptr, nullptr };
            }

            break;

        default :
            DBG_panic(("pArray->u.array.type..", pArray->u.array.type));
        }

        pArray->u.array.alloc_size = iNewAllocSize;
    }

    pArray->u.array.size = iNewSize;
}

void Native::ArrayInsert(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pArray, int iPos, DVM_Value value)
{
    ArrayResize(pVirtualMachine, pArray, pArray->u.array.size + 1);

    switch (pArray->u.array.type)
    {
    case INT_ARRAY :
        memmove_s(&pArray->u.array.u.int_array[iPos + 1], sizeof(int) * (pArray->u.array.size - iPos),
            &pArray->u.array.u.int_array[iPos], sizeof(int) * (pArray->u.array.size - iPos));
        pArray->u.array.u.int_array[iPos] = value.int_value;
        break;

    case DOUBLE_ARRAY :
        memmove_s(&pArray->u.array.u.double_array[iPos + 1], sizeof(double) * (pArray->u.array.size - iPos),
            &pArray->u.array.u.double_array[iPos], sizeof(double) * (pArray->u.array.size - iPos));
        pArray->u.array.u.double_array[iPos] = value.double_value;
        break;

    case OBJECT_ARRAY :
        memmove_s(&pArray->u.array.u.object[iPos + 1], sizeof(DVM_ObjectRef) * (pArray->u.array.size - iPos),
            &pArray->u.array.u.object[iPos], sizeof(DVM_ObjectRef) * (pArray->u.array.size - iPos));
        pArray->u.array.u.object[iPos] = value.object;
        break;

    default:
        DBG_panic(("pArray->u.array.type..", pArray->u.array.type));
    }

    pArray->u.array.size++;
}

void Native::ArrayRemove(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pArray, int iPos)
{
    switch (pArray->u.array.type)
    {
    case INT_ARRAY :
        memmove_s(&pArray->u.array.u.int_array[iPos], sizeof(int) * (pArray->u.array.size - iPos - 1),
            &pArray->u.array.u.int_array[iPos + 1], sizeof(int) * (pArray->u.array.size - iPos - 1));
        break;

    case DOUBLE_ARRAY :
        memmove_s(&pArray->u.array.u.double_array[iPos], sizeof(double) * (pArray->u.array.size - iPos - 1),
            &pArray->u.array.u.double_array[iPos + 1], sizeof(double) * (pArray->u.array.size - iPos - 1));
        break;

    case OBJECT_ARRAY :
        memmove_s(&pArray->u.array.u.object[iPos], sizeof(DVM_ObjectRef) * (pArray->u.array.size - iPos - 1),
            &pArray->u.array.u.object[iPos + 1], sizeof(DVM_ObjectRef) * (pArray->u.array.size - iPos - 1));
        break;
    }

    ArrayResize(pVirtualMachine, pArray, pArray->u.array.size - 1);
    pArray->u.array.size--;
}