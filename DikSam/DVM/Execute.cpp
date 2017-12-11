#include "StdAfx.h"
#include "Execute.h"
#include "Debug.h"
#include "Memory.h"
#include "Util.h"
#include "Error.h"
#include "Interface.h"
#include "Load.h"
#include "DVM_pri.h"
#include "Dispose.h"
#include "OpcodeInfo.h"

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

#ifdef MEM_free
#undef MEM_free
#endif
#define MEM_free(ptr)                       (m_Memory.Free(ptr))

#ifdef MEM_strdup
#undef MEM_strdup
#endif
#define MEM_strdup(ptr)                     (m_Memory.StrDUP(__FILE__, __LINE__, ptr))


#define GET_2BYTE_INT(p)            (((p)[0] << 8) + (p)[1])
#define SET_2BYTE_INT(p, value)     (((p)[0] = (value) >> 8), ((p)[1] = value & 0xff))

#define STI(sp)     ((pVirtualMachine)->stack.stack[(pVirtualMachine)->stack.stack_pointer + (sp)].int_value)
#define STD(sp)     ((pVirtualMachine)->stack.stack[(pVirtualMachine)->stack.stack_pointer + (sp)].double_value)
#define STO(sp)     ((pVirtualMachine)->stack.stack[(pVirtualMachine)->stack.stack_pointer + (sp)].object)

#define STI_I(sp)   ((pVirtualMachine)->stack.stack[(sp)].int_value)
#define STD_I(sp)   ((pVirtualMachine)->stack.stack[(sp)].double_value)
#define STO_I(sp)   ((pVirtualMachine)->stack.stack[(sp)].object)

#define STI_WRITE(sp, r) \
    ((pVirtualMachine)->stack.stack[(pVirtualMachine)->stack.stack_pointer + (sp)].int_value = r, \
    (pVirtualMachine)->stack.pointer_flags[(pVirtualMachine)->stack.stack_pointer + (sp)] = DVM_FALSE)
#define STD_WRITE(sp, r) \
    ((pVirtualMachine)->stack.stack[(pVirtualMachine)->stack.stack_pointer + (sp)].double_value = r, \
    (pVirtualMachine)->stack.pointer_flags[(pVirtualMachine)->stack.stack_pointer + (sp)] = DVM_FALSE)
#define STO_WRITE(sp, r) \
    ((pVirtualMachine)->stack.stack[(pVirtualMachine)->stack.stack_pointer + (sp)].object = r, \
    (pVirtualMachine)->stack.pointer_flags[(pVirtualMachine)->stack.stack_pointer + (sp)] = DVM_TRUE)

#define STI_WRITE_I(sp, r) \
    ((pVirtualMachine)->stack.stack[(sp)].int_value = r, \
    (pVirtualMachine)->stack.pointer_flags[(sp)] = DVM_FALSE)
#define STD_WRITE_I(sp, r) \
    ((pVirtualMachine)->stack.stack[(sp)].double_value = r, \
    (pVirtualMachine)->stack.pointer_flags[(sp)] = DVM_FALSE)
#define STO_WRITE_I(sp, r) \
    ((pVirtualMachine)->stack.stack[(sp)].object = r, \
    (pVirtualMachine)->stack.pointer_flags[(sp)] = DVM_TRUE)

Execute::Execute(Debug& debug, Memory& memory, Util& util, Error& error, Interface& interfaceRef)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Util(util)
    , m_Error(error)
    , m_Interface(interfaceRef)
    , m_Heap(debug, memory, util)
{
}

Execute::~Execute()
{
}

DVM_Value Execute::operator () (DVM_VirtualMachine *pVirtualMachine)
{
    pVirtualMachine->current_executable = pVirtualMachine->top_level;
    pVirtualMachine->current_function = nullptr;
    pVirtualMachine->pc = 0;

    ExpandStack(pVirtualMachine, pVirtualMachine->top_level->executable->need_stack_size);
    DVM_Value ret = ExecuteCode(pVirtualMachine, nullptr, pVirtualMachine->top_level->executable->code, pVirtualMachine->top_level->executable->code_size, 0);

    return ret;
}

void Execute::DisposeExecutableList(DVM_ExecutableList *pList)
{
    while (pList->list)
    {
        DVM_ExecutableItem *pTemp = pList->list;

        pList->list = pTemp->next;
        Dispose(m_Debug, m_Memory)(pTemp->executable);
        MEM_free(pTemp);
    }

    MEM_free(pList);
}

void Execute::DisposeVirtualMachine(DVM_VirtualMachine *pVirtualMachine)
{
    while (pVirtualMachine->executable_entry)
    {
        ExecutableEntry *pTemp = pVirtualMachine->executable_entry;
        pVirtualMachine->executable_entry = pTemp->next;

        MEM_free(pTemp->static_v.variable);
        MEM_free(pTemp);
    }

    m_Heap.GarbageCollect(pVirtualMachine);

    MEM_free(pVirtualMachine->stack.stack);
    MEM_free(pVirtualMachine->stack.pointer_flags);

    for (int i = 0; i < pVirtualMachine->function_count; i++)
    {
        MEM_free(pVirtualMachine->function[i]->name);
        MEM_free(pVirtualMachine->function[i]->package_name);
        MEM_free(pVirtualMachine->function[i]);
    }

    MEM_free(pVirtualMachine->function);

    for (int i = 0; i < pVirtualMachine->class_count; i++)
    {
        MEM_free(pVirtualMachine->classes[i]->package_name);
        MEM_free(pVirtualMachine->classes[i]->name);

        DisposeVTable(pVirtualMachine->classes[i]->class_table);

        for (int j = 0; j < pVirtualMachine->classes[i]->interface_count; j++)
        {
            DisposeVTable(pVirtualMachine->classes[i]->interface_v_table[j]);
        }

        MEM_free(pVirtualMachine->classes[i]->interface_v_table);
        MEM_free(pVirtualMachine->classes[i]->interface_);
        MEM_free(pVirtualMachine->classes[i]->field_type);
        MEM_free(pVirtualMachine->classes[i]);
    }

    MEM_free(pVirtualMachine->array_v_table->table);
    MEM_free(pVirtualMachine->array_v_table);

    MEM_free(pVirtualMachine->string_v_table->table);
    MEM_free(pVirtualMachine->string_v_table);

    MEM_free(pVirtualMachine->classes);

    MEM_free(pVirtualMachine);
}

int Execute::ArrayGetInt(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef array, int iIndex)
{
    CheckArray(array, iIndex, pVirtualMachine->current_executable->executable, pVirtualMachine->current_function, pVirtualMachine->pc);

    return array.data->u.array.u.int_array[iIndex];
}

double Execute::ArrayGetDouble(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef array, int iIndex)
{
    CheckArray(array, iIndex, pVirtualMachine->current_executable->executable, pVirtualMachine->current_function, pVirtualMachine->pc);

    return array.data->u.array.u.double_array[iIndex];
}

DVM_ObjectRef Execute::ArrayGetObject(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef array, int iIndex)
{
    CheckArray(array, iIndex, pVirtualMachine->current_executable->executable, pVirtualMachine->current_function, pVirtualMachine->pc);

    return array.data->u.array.u.object[iIndex];
}

void Execute::ArraySetInt(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef array, int iIndex, int value)
{
    CheckArray(array, iIndex, pVirtualMachine->current_executable->executable, pVirtualMachine->current_function, pVirtualMachine->pc);

    array.data->u.array.u.int_array[iIndex] = value;
}

void Execute::ArraySetDouble(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef array, int iIndex, double value)
{
    CheckArray(array, iIndex, pVirtualMachine->current_executable->executable, pVirtualMachine->current_function, pVirtualMachine->pc);

    array.data->u.array.u.double_array[iIndex] = value;
}

void Execute::ArraySetObject(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef array, int iIndex, DVM_ObjectRef value)
{
    CheckArray(array, iIndex, pVirtualMachine->current_executable->executable, pVirtualMachine->current_function, pVirtualMachine->pc);

    array.data->u.array.u.object[iIndex] = value;
}

void Execute::CheckArray(DVM_ObjectRef array, int iIndex, DVM_Executable *pExecutable, Function *pFunction, int iPC)
{
    if (nullptr == array.data)
    {
        m_Error.DVMError(pExecutable, pFunction, iPC, NULL_POINTER_ERR, MESSAGE_ARGUMENT_END);
    }

    if (iIndex < 0 || iIndex >= array.data->u.array.size)
    {
        m_Error.DVMError(pExecutable, pFunction, iPC,
            INDEX_OUT_OF_BOUNDS_ERR, INT_MESSAGE_ARGUMENT, "index", iIndex, INT_MESSAGE_ARGUMENT, "size", array.data->u.array.size,
            MESSAGE_ARGUMENT_END);
    }
}

DVM_ObjectRef Execute::ChainString(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef str1, DVM_ObjectRef str2)
{
    int iLen1 = 0, iLen2 = 0;

    if (str1.data)
    {
        iLen1 = str1.data->u.string.string ? std::wstring(str1.data->u.string.string).length() : 0;
    }

    if (str2.data)
    {
        iLen2 = str2.data->u.string.string ? std::wstring(str2.data->u.string.string).length() : 0;
    }
    
    DVM_Char *str = (DVM_Char*)MEM_malloc(sizeof(DVM_Char) * (iLen1 + iLen2 + 1));

    int i = 0;
    for (i = 0; i < iLen1; i++)
    {
        str[i] = str1.data->u.string.string[i];
    }

    for (i = iLen1; i < iLen1 + iLen2; i++)
    {
        str[i] = str2.data->u.string.string[i - iLen1];
    }

    str[i] = 0;

    DVM_ObjectRef ret = m_Heap.CreateString(pVirtualMachine, str);

    return ret;
}

void Execute::InitializeLocalVariables(DVM_VirtualMachine *pVirtualMachine, DVM_Function *pFunction, int iFromSP)
{
    for (int i = 0, idx = iFromSP; i < pFunction->local_variable_count; i++, idx++)
    {
        pVirtualMachine->stack.pointer_flags[idx] = DVM_FALSE;
    }

    for (int i = 0, idx = iFromSP; i < pFunction->local_variable_count; i++, idx++)
    {
        m_Util.InitializeValue(pFunction->local_variable[i].type, &pVirtualMachine->stack.stack[idx]);

        if (IsPointerType(pFunction->local_variable[i].type))
        {
            pVirtualMachine->stack.pointer_flags[idx] = DVM_TRUE;
        }
    }
}

void Execute::ExpandStack(DVM_VirtualMachine *pVirtualMachine, int iNeedStackSize)
{
    int iRest = pVirtualMachine->stack.alloc_size - pVirtualMachine->stack.stack_pointer;

    if (iRest > iNeedStackSize)
        return;

    int iRevalueUp = ((iRest / STACK_ALLOC_SIZE) + 1) * STACK_ALLOC_SIZE;

    pVirtualMachine->stack.alloc_size += iRevalueUp;
    pVirtualMachine->stack.stack = (DVM_Value*)MEM_realloc(pVirtualMachine->stack.stack, sizeof(DVM_Value) * pVirtualMachine->stack.alloc_size);
    pVirtualMachine->stack.pointer_flags = (DVM_Boolean*)MEM_realloc(pVirtualMachine->stack.pointer_flags, sizeof(DVM_Boolean) * pVirtualMachine->stack.alloc_size);
}

void Execute::InvokeNativeFunction(DVM_VirtualMachine *pVirtualMachine, Function *pCaller, Function *pCallee, int iPC, int *pSP, int iBase)
{
    DBG_assert(NATIVE_FUNCTION == pCallee->kind, ("pCallee->kind..", pCallee->kind));

    (*pSP)--;
    DVM_Value *stack = pVirtualMachine->stack.stack;

    int iArgCount = 0;
    if (DVM_TRUE == pCallee->u.native_f.is_method)
    {
        iArgCount = pCallee->u.native_f.arg_count + 1;
    }
    else
    {
        iArgCount = pCallee->u.native_f.arg_count;
    }

    CallInfo *pCallInfo = (CallInfo*)&pVirtualMachine->stack.stack[*pSP];

    pCallInfo->caller = pCaller;
    pCallInfo->caller_address = iPC;
    pCallInfo->base = iBase;

    for (int i = 0; i < CALL_INFO_ALIGN_SIZE; i++)
    {
        pVirtualMachine->stack.pointer_flags[*pSP + i] = DVM_FALSE;
    }

    *pSP += CALL_INFO_ALIGN_SIZE;
    pVirtualMachine->current_function = pCallee;
    
    DVM_Value ret = pCallee->u.native_f.proc(pCallee->u.native_f.pThis,
        pVirtualMachine, pCallee->u.native_f.arg_count,
        &stack[*pSP - iArgCount - CALL_INFO_ALIGN_SIZE]);

    pVirtualMachine->current_function = pCaller;
    *pSP -= iArgCount + CALL_INFO_ALIGN_SIZE;
    stack[*pSP] = ret;
    pVirtualMachine->stack.pointer_flags[*pSP] = pCallee->u.native_f.return_pointer;
    (*pSP)++;
}

void Execute::InvokeDikSamFunction(DVM_VirtualMachine *pVirtualMachine, Function **ppCaller, Function *pCallee, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pSP, int *pBase, ExecutableEntry **ppExeEntry, DVM_Executable **ppExe)
{
    if (DVM_FALSE == pCallee->is_implemented)
    {
        Load load(m_Debug, m_Memory, m_Util, m_Error, m_Interface);
        load.DynamicLoad(pVirtualMachine, *ppExe, *ppCaller, *pPC, pCallee);
    }

    *ppExeEntry = pCallee->u.diksam_f.executable;
    *ppExe = (*ppExeEntry)->executable;

    DVM_Function *pCalleeFunction = &(*ppExe)->function[pCallee->u.diksam_f.index];

    ExpandStack(pVirtualMachine, CALL_INFO_ALIGN_SIZE + pCalleeFunction->local_variable_count + (*ppExe)->function[pCallee->u.diksam_f.index].need_stack_size);

    CallInfo *pCallInfo = (CallInfo*)&pVirtualMachine->stack.stack[(*pSP - 1)];

    pCallInfo->caller = *ppCaller;
    pCallInfo->caller_address = *pPC;
    pCallInfo->base = *pBase;

    for (int i = 0; i < CALL_INFO_ALIGN_SIZE; i++)
    {
        pVirtualMachine->stack.pointer_flags[(*pSP - 1) + i] = DVM_FALSE;
    }

    *pBase = (*pSP - 1) - pCalleeFunction->parameter_count;

    if (DVM_TRUE == pCalleeFunction->is_method)
    {
        // for this
        (*pBase)--;
    }

    *ppCaller = pCallee;

    InitializeLocalVariables(pVirtualMachine, pCalleeFunction, (*pSP - 1) + CALL_INFO_ALIGN_SIZE);

    *pSP = (*pSP - 1);
    *pSP += CALL_INFO_ALIGN_SIZE + pCalleeFunction->local_variable_count;
    *pPC = 0;

    *ppCode = (*ppExe)->function[pCallee->u.diksam_f.index].code;
    *pCodeSize = (*ppExe)->function[pCallee->u.diksam_f.index].code_size;
}

DVM_Boolean Execute::DoReturn(DVM_VirtualMachine *pVirtualMachine, Function **ppFunction, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pBase, ExecutableEntry **ppExeEntry, DVM_Executable **ppExe)
{
    DVM_Function *pCallee = &(*ppExe)->function[(*ppFunction)->u.diksam_f.index];
    
    int iArgCount = pCallee->parameter_count;
    if (DVM_TRUE == pCallee->is_method)
    {
        // fot this
        iArgCount++;
    }

    CallInfo *pCallInfo = (CallInfo*)&pVirtualMachine->stack.stack[*pBase + iArgCount];

    if (pCallInfo->caller)
    {
        *ppExeEntry = pCallInfo->caller->u.diksam_f.executable;
        *ppExe = (*ppExeEntry)->executable;

        if (DIKSAM_FUNCTION == pCallInfo->caller->kind)
        {
            DVM_Function *pCaller = &(*ppExe)->function[pCallInfo->caller->u.diksam_f.index];

            *ppCode = pCaller->code;
            *pCodeSize = pCaller->code_size;
        }
    }
    else
    {
        *ppExeEntry = pVirtualMachine->top_level;
        *ppExe = pVirtualMachine->top_level->executable;
        *ppCode = pVirtualMachine->top_level->executable->code;
        *pCodeSize = pVirtualMachine->top_level->executable->code_size;
    }

    *ppFunction = pCallInfo->caller;

    pVirtualMachine->stack.stack_pointer = *pBase;
    *pPC = pCallInfo->caller_address + 1;
    *pBase = pCallInfo->base;

    return (CALL_FROM_NATIVE == pCallInfo->caller_address ? DVM_TRUE : DVM_FALSE);
}

DVM_Boolean Execute::ReturnFunction(DVM_VirtualMachine *pVirtualMachine, Function **ppFunction, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pBase, ExecutableEntry **ppExeEntry, DVM_Executable **ppExe)
{
    DVM_Value returnValue = pVirtualMachine->stack.stack[pVirtualMachine->stack.stack_pointer - 1];
    pVirtualMachine->stack.stack_pointer--;

    DVM_Function *pCallee = &(*ppExe)->function[(*ppFunction)->u.diksam_f.index];
    DVM_Boolean ret = DoReturn(pVirtualMachine, ppFunction, ppCode, pCodeSize, pPC, pBase, ppExeEntry, ppExe);
    
    pVirtualMachine->stack.stack[pVirtualMachine->stack.stack_pointer] = returnValue;
    pVirtualMachine->stack.pointer_flags[pVirtualMachine->stack.stack_pointer] = IsPointerType(pCallee->type) ? DVM_TRUE : DVM_FALSE;
    pVirtualMachine->stack.stack_pointer++;

    return ret;
}

DVM_ObjectRef Execute::CreateArraySub(DVM_VirtualMachine *pVirtualMachine, int iDim, int iDimIndex, DVM_TypeSpecifier *pType)
{
    DVM_ObjectRef ret{ nullptr, nullptr };
    int size = STI(-iDim);

    if (iDimIndex == pType->derive_count - 1)
    {
        switch (pType->basic_type)
        {
        case DVM_VOID_TYPE :
            DBG_panic(("create void array"));
            break;

        case DVM_BOOLEAN_TYPE :
        case DVM_INT_TYPE :
            ret = m_Heap.CreateArrayInt(pVirtualMachine, size);
            break;

        case DVM_DOUBLE_TYPE :
            ret = m_Heap.CreateArrayDouble(pVirtualMachine, size);
            break;

        case DVM_STRING_TYPE :
        case DVM_CLASS_TYPE :
            ret = m_Heap.CreateArrayObject(pVirtualMachine, size);
            break;

        case DVM_NULL_TYPE :
        case DVM_BASE_TYPE :
        default :
            DBG_assert(0, ("pType->basic_type..", pType->basic_type));
        }
    }
    else if (DVM_FUNCTION_DERIVE == pType->derive[iDimIndex].tag)
    {
        DBG_panic(("Function type in array literal."));
    }
    else
    {
        ret = m_Heap.CreateArrayObject(pVirtualMachine, size);

        if (iDimIndex < iDim - 1)
        {
            STO_WRITE(0, ret);
            pVirtualMachine->stack.stack_pointer++;

            for (int i = 0; i < size; i++) {
                DVM_ObjectRef child = CreateArraySub(pVirtualMachine, iDim, iDimIndex + 1, pType);
                ArraySetObject(pVirtualMachine, ret, i, child);
            }

            pVirtualMachine->stack.stack_pointer--;
        }
    }

    return ret;
}

DVM_ObjectRef Execute::CreateArray(DVM_VirtualMachine *pVirtualMachine, int iDim, DVM_TypeSpecifier *pType)
{
    return CreateArraySub(pVirtualMachine, iDim, 0, pType);
}

DVM_ObjectRef Execute::CreateArrayLiteralInt(DVM_VirtualMachine *pVirtualMachine, int iSize)
{
    DVM_ObjectRef array = m_Heap.CreateArrayInt(pVirtualMachine, iSize);

    for (int i = 0; i < iSize; i++)
    {
        array.data->u.array.u.int_array[i] = STI(-iSize + i);
    }

    return array;
}

DVM_ObjectRef Execute::CreateArrayLiteralDouble(DVM_VirtualMachine *pVirtualMachine, int iSize)
{
    DVM_ObjectRef array = m_Heap.CreateArrayDouble(pVirtualMachine, iSize);

    for (int i = 0; i < iSize; i++)
    {
        array.data->u.array.u.double_array[i] = STD(-iSize + i);
    }

    return array;
}

DVM_ObjectRef Execute::CreateArrayLiteralObject(DVM_VirtualMachine *pVirtualMachine, int iSize)
{
    DVM_ObjectRef array = m_Heap.CreateArrayObject(pVirtualMachine, iSize);

    for (int i = 0; i < iSize; i++)
    {
        array.data->u.array.u.object[i] = STO(-iSize + i);
    }

    return array;
}

void Execute::RestorePC(DVM_VirtualMachine *pVirtualMachine, ExecutableEntry *pExecutableEntry, Function *pFunction, int iPC)
{
    pVirtualMachine->current_executable = pExecutableEntry;
    pVirtualMachine->current_function = pFunction;
    pVirtualMachine->pc = iPC;
}

DVM_Value Execute::ExecuteCode(DVM_VirtualMachine *pVirtualMachine, Function *pFunction, DVM_Byte *pCode, int iCodeSize, int iBase)
{
    DVM_Executable *exe = pVirtualMachine->current_executable->executable;
    ExecutableEntry *ee = pVirtualMachine->current_executable;
    int pc = pVirtualMachine->pc;
    DVM_Value ret;
    bool bReturn = false;

    while (pc < iCodeSize && !bReturn)
    {
        switch (pCode[pc])
        {
        case DVM_PUSH_INT_1BYTE :
            STI_WRITE(0, pCode[pc + 1]);
            pVirtualMachine->stack.stack_pointer++;
            pc += 2;
            break;

        case DVM_PUSH_INT_2BYTE :
            STI_WRITE(0, GET_2BYTE_INT(&pCode[pc + 1]));
            pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_INT :
            STI_WRITE(0, exe->constant_pool[GET_2BYTE_INT(&pCode[pc + 1])].u.c_int);
            pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_DOUBLE_0 :
            STD_WRITE(0, 0.0);
            pVirtualMachine->stack.stack_pointer++;
            pc++;
            break;

        case DVM_PUSH_DOUBLE_1 :
            STD_WRITE(0, 1.0);
            pVirtualMachine->stack.stack_pointer++;
            pc++;
            break;

        case DVM_PUSH_DOUBLE :
            STD_WRITE(0, exe->constant_pool[GET_2BYTE_INT(&pCode[pc + 1])].u.c_double);
            pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_STRING :
            STO_WRITE(0, m_Heap.LiteralToString(pVirtualMachine, exe->constant_pool[GET_2BYTE_INT(&pCode[pc + 1])].u.c_string));
            pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_NULL:
        {
                DVM_ObjectRef nullRef{ nullptr, nullptr };
                
                STO_WRITE(0, nullRef);
                pVirtualMachine->stack.stack_pointer++;
                pc++; 
            }
            break;

        case DVM_PUSH_STACK_INT :
            STI_WRITE(0, STI_I(iBase + GET_2BYTE_INT(&pCode[pc + 1])));
            pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_STACK_DOUBLE :
            STD_WRITE(0, STD_I(iBase + GET_2BYTE_INT(&pCode[pc + 1])));
            pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_STACK_OBJECT :
            STO_WRITE(0, STO_I(iBase + GET_2BYTE_INT(&pCode[pc + 1])));
            pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_POP_STACK_INT :
            STI_WRITE_I(iBase + GET_2BYTE_INT(&pCode[pc + 1]), STI(-1));
            pVirtualMachine->stack.stack_pointer--;
            pc += 3;
            break;

        case DVM_POP_STACK_DOUBLE :
            STD_WRITE_I(iBase + GET_2BYTE_INT(&pCode[pc + 1]), STD(-1));
            pVirtualMachine->stack.stack_pointer--;
            pc += 3;
            break;

        case DVM_POP_STACK_OBJECT :
            STO_WRITE_I(iBase + GET_2BYTE_INT(&pCode[pc + 1]), STO(-1));
            pVirtualMachine->stack.stack_pointer--;
            pc += 3;
            break;

        case DVM_PUSH_STATIC_INT :
            STI_WRITE(0, ee->static_v.variable[GET_2BYTE_INT(&pCode[pc + 1])].int_value);
            pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_STATIC_DOUBLE :
            STD_WRITE(0, ee->static_v.variable[GET_2BYTE_INT(&pCode[pc + 1])].double_value);
            pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_STATIC_OBJECT :
            STO_WRITE(0, ee->static_v.variable[GET_2BYTE_INT(&pCode[pc + 1])].object);
            pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_POP_STATIC_INT :
            ee->static_v.variable[GET_2BYTE_INT(&pCode[pc + 1])].int_value = STI(-1);
            pVirtualMachine->stack.stack_pointer--;
            pc += 3;
            break;

        case DVM_POP_STATIC_DOUBLE :
            ee->static_v.variable[GET_2BYTE_INT(&pCode[pc + 1])].double_value = STD(-1);
            pVirtualMachine->stack.stack_pointer--;
            pc += 3;
            break;

        case DVM_POP_STATIC_OBJECT :
            ee->static_v.variable[GET_2BYTE_INT(&pCode[pc + 1])].object = STO(-1);
            pVirtualMachine->stack.stack_pointer--;
            pc += 3;
            break;

        case DVM_PUSH_ARRAY_INT :
            {
                DVM_ObjectRef array = STO(-2);
                int index = STI(-1);

                RestorePC(pVirtualMachine, ee, pFunction, pc);
                int value = ArrayGetInt(pVirtualMachine, array, index);

                STI_WRITE(-2, value);
                pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_PUSH_ARRAY_DOUBLE :
            {
                DVM_ObjectRef array = STO(-2);
                int index = STI(-1);

                RestorePC(pVirtualMachine, ee, pFunction, pc);
                double value = ArrayGetDouble(pVirtualMachine, array, index);

                STD_WRITE(-2, value);
                pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_PUSH_ARRAY_OBJECT :
            {
                DVM_ObjectRef array = STO(-2);
                int index = STI(-1);

                RestorePC(pVirtualMachine, ee, pFunction, pc);
                DVM_ObjectRef value = ArrayGetObject(pVirtualMachine, array, index);

                STO_WRITE(-2, value);
                pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_POP_ARRAY_INT :
            {
                int value = STI(-3);
                DVM_ObjectRef array = STO(-2);
                int index = STI(-1);

                RestorePC(pVirtualMachine, ee, pFunction, pc);
                ArraySetInt(pVirtualMachine, array, index, value);

                pVirtualMachine->stack.stack_pointer -= 3;
                pc++;
            }
            break;

        case DVM_POP_ARRAY_DOUBLE :
            {
                double value = STD(-3);
                DVM_ObjectRef array = STO(-2);
                int index = STI(-1);

                RestorePC(pVirtualMachine, ee, pFunction, pc);
                ArraySetDouble(pVirtualMachine, array, index, value);

                pVirtualMachine->stack.stack_pointer -= 3;
                pc++;
            }
            break;

        case DVM_POP_ARRAY_OBJECT :
            {
                DVM_ObjectRef value = STO(-3);
                DVM_ObjectRef array = STO(-2);
                int index = STI(-1);

                RestorePC(pVirtualMachine, ee, pFunction, pc);
                ArraySetObject(pVirtualMachine, array, index, value);

                pVirtualMachine->stack.stack_pointer -= 3;
                pc++;
            }
            break;

        case DVM_PUSH_FIELD_INT :
            {
                DVM_ObjectRef value = STO(-1);
                int index = GET_2BYTE_INT(&pCode[pc + 1]);

                CheckNullPointer(exe, pFunction, pc, &value);
                STI_WRITE(-1, value.data->u.class_object.field[index].int_value);

                pc += 3;
            }
            break;

        case DVM_PUSH_FIELD_DOUBLE :
            {
                DVM_ObjectRef value = STO(-1);
                int index = GET_2BYTE_INT(&pCode[pc + 1]);

                CheckNullPointer(exe, pFunction, pc, &value);
                STD_WRITE(-1, value.data->u.class_object.field[index].double_value);

                pc += 3;
            }
            break;

        case DVM_PUSH_FIELD_OBJECT :
            {
                DVM_ObjectRef value = STO(-1);
                int index = GET_2BYTE_INT(&pCode[pc + 1]);

                CheckNullPointer(exe, pFunction, pc, &value);
                STO_WRITE(-1, value.data->u.class_object.field[index].object);

                pc += 3;
            }
            break;

        case DVM_POP_FIELD_INT :
            {
                DVM_ObjectRef value = STO(-1);
                int index = GET_2BYTE_INT(&pCode[pc + 1]);

                CheckNullPointer(exe, pFunction, pc, &value);
                value.data->u.class_object.field[index].int_value = STI(-2);

                pVirtualMachine->stack.stack_pointer -= 2;
                pc += 3;
            }
            break;

            case DVM_POP_FIELD_DOUBLE :
            {
                DVM_ObjectRef value = STO(-1);
                int index = GET_2BYTE_INT(&pCode[pc + 1]);

                CheckNullPointer(exe, pFunction, pc, &value);
                value.data->u.class_object.field[index].double_value = STD(-2);

                pVirtualMachine->stack.stack_pointer -= 2;
                pc += 3;
            }
            break;

            case DVM_POP_FIELD_OBJECT :
            {
                DVM_ObjectRef value = STO(-1);
                int index = GET_2BYTE_INT(&pCode[pc + 1]);

                CheckNullPointer(exe, pFunction, pc, &value);
                value.data->u.class_object.field[index].object = STO(-2);

                pVirtualMachine->stack.stack_pointer -= 2;
                pc += 3;
            }
            break;

        case DVM_ADD_INT :
            STI(-2) = STI(-2) + STI(-1);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_ADD_DOUBLE :
            STD(-2) = STD(-2) + STD(-1);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_ADD_STRING :
            STO(-2) = ChainString(pVirtualMachine, STO(-2), STO(-1));
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_SUB_INT :
            STI(-2) = STI(-2) - STI(-1);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_SUB_DOUBLE :
            STD(-2) = STD(-2) - STD(-1);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_MUL_INT :
            STI(-2) = STI(-2) * STI(-1);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_MUL_DOUBLE :
            STD(-2) = STD(-2) * STD(-1);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_DIV_INT :
            if (0 == STI(-1))
            {
                m_Error.DVMError(exe, pFunction, pc, DIVISION_BY_ZERO_ERR, MESSAGE_ARGUMENT_END);
            }

            STI(-2) = STI(-2) / STI(-1);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_DIV_DOUBLE :
            STD(-2) = STD(-2) / STD(-1);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_MOD_INT :
            STI(-2) = STI(-2) % STI(-1);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_MOD_DOUBLE :
            STD(-2) = std::fmod(STD(-2), STD(-1));
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_MINUS_INT :
            STI(-1) = -STI(-1);
            pc++;
            break;

        case DVM_MINUS_DOUBLE :
            STD(-1) = -STD(-1);
            pc++;
            break;

        case DVM_INCREMENT :
            STI(-1)++;
            pc++;
            break;

        case DVM_DECREMENT :
            STI(-1)--;
            pc++;
            break;

        case DVM_CAST_INT_TO_DOUBLE :
            STD(-1) = (double)STI(-1);
            pc++;
            break;

        case DVM_CAST_DOUBLE_TO_INT :
            STI(-1) = (int)STD(-1);
            pc++;
            break;

        case DVM_CAST_BOOLEAN_TO_STRING :
            if (DVM_TRUE == STI(-1))
            {
                STO_WRITE(-1, m_Heap.LiteralToString(pVirtualMachine, TRUE_STRING));
            }
            else
            {
                STO_WRITE(-1, m_Heap.LiteralToString(pVirtualMachine, FALSE_STRING));
            }
            pc++;
            break;

        case DVM_CAST_INT_TO_STRING :
            {
                std::wstringstream ss;
                
                ss << STI(-1);
                std::wstring wstr(ss.str());

                DVM_Char *str = (DVM_Char*)MEM_malloc(sizeof(DVM_Char) * (wstr.length() + 1));
                
                for (size_t i = 0; i < wstr.length(); i++)
                {
                    str[i] = wstr[i];
                }

                str[wstr.length()] = 0;

                STO_WRITE(-1, m_Heap.CreateString(pVirtualMachine, str));
                pc++;
            }
            break;

        case DVM_CAST_DOUBLE_TO_STRING :
            {
                std::wstringstream ss;
                
                ss << STD(-1);
                std::wstring wstr(ss.str());

                DVM_Char *str = (DVM_Char*)MEM_malloc(sizeof(DVM_Char) * (wstr.length() + 1));
                
                for (size_t i = 0; i < wstr.length(); i++)
                {
                    str[i] = wstr[i];
                }

                str[wstr.length()] = 0;

                STO_WRITE(-1, m_Heap.CreateString(pVirtualMachine, str));
                pc++;
            }
            break;

        case DVM_UP_CAST :
            {
                DVM_ObjectRef value = STO(-1);
                int index = GET_2BYTE_INT(&pCode[pc + 1]);

                CheckNullPointer(exe, pFunction, pc, &value);
                value.v_table = value.v_table->exec_class->interface_v_table[index];

                STO_WRITE(-1, value);

                pc += 3;
            }
            break;

        case DVM_DOWN_CAST :
            {
                DVM_ObjectRef value = STO(-1);
                int index = GET_2BYTE_INT(&pCode[pc + 1]);

                CheckNullPointer(exe, pFunction, pc, &value);

                DVM_Boolean isSameClass;
                DVM_Boolean isInterface;
                int iInterfaceIdx;

                CheckDownCast(pVirtualMachine, exe, pFunction, pc, &value, index, &isSameClass, &isInterface, &iInterfaceIdx);

                if (DVM_FALSE == isSameClass)
                {
                    if (DVM_TRUE == isInterface)
                    {
                        value.v_table = value.v_table->exec_class->interface_v_table[iInterfaceIdx];
                    }
                    else
                    {
                        value.v_table = value.v_table->exec_class->class_table;
                    }
                }

                STO_WRITE(-1, value);

                pc += 3;
            }
            break;

        case DVM_EQ_INT :
            STI(-2) = (STI(-2) == STI(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_EQ_DOUBLE :
            STI(-2) = (STD(-2) == STD(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_EQ_OBJECT :
            STI_WRITE(-2, STO(-2).data == STO(-1).data ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_EQ_STRING :
            {
                std::basic_string<DVM_Char> str1(STO(-2).data->u.string.string ? STO(-2).data->u.string.string : L"");
                std::basic_string<DVM_Char> str2(STO(-1).data->u.string.string ? STO(-1).data->u.string.string : L"");
                
                STI_WRITE(-2, str1 == str2 ? DVM_TRUE : DVM_FALSE);
                pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_GT_INT :
            STI(-2) = (STI(-2) > STI(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_GT_DOUBLE :
            STI(-2) = (STD(-2) > STD(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_GT_STRING :
            {
                std::basic_string<DVM_Char> str1(STO(-2).data->u.string.string ? STO(-2).data->u.string.string : L"");
                std::basic_string<DVM_Char> str2(STO(-1).data->u.string.string ? STO(-1).data->u.string.string : L"");
                
                STI_WRITE(-2, str1 > str2 ? DVM_TRUE : DVM_FALSE);
                pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_GE_INT :
            STI(-2) = (STI(-2) >= STI(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_GE_DOUBLE :
            STI(-2) = (STD(-2) >= STD(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_GE_STRING :
            {
                std::basic_string<DVM_Char> str1(STO(-2).data->u.string.string ? STO(-2).data->u.string.string : L"");
                std::basic_string<DVM_Char> str2(STO(-1).data->u.string.string ? STO(-1).data->u.string.string : L"");
                
                STI_WRITE(-2, str1 >= str2 ? DVM_TRUE : DVM_FALSE);
                pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_LT_INT :
            STI(-2) = (STI(-2) < STI(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_LT_DOUBLE :
            STI(-2) = (STD(-2) < STD(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_LT_STRING :
            {
                std::basic_string<DVM_Char> str1(STO(-2).data->u.string.string ? STO(-2).data->u.string.string : L"");
                std::basic_string<DVM_Char> str2(STO(-1).data->u.string.string ? STO(-1).data->u.string.string : L"");
                
                STI_WRITE(-2, str1 < str2 ? DVM_TRUE : DVM_FALSE);
                pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_LE_INT :
            STI(-2) = (STI(-2) <= STI(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_LE_DOUBLE :
            STI(-2) = (STD(-2) <= STD(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_LE_STRING :
            {
                std::basic_string<DVM_Char> str1(STO(-2).data->u.string.string ? STO(-2).data->u.string.string : L"");
                std::basic_string<DVM_Char> str2(STO(-1).data->u.string.string ? STO(-1).data->u.string.string : L"");
                
                STI_WRITE(-2, str1 <= str2 ? DVM_TRUE : DVM_FALSE);
                pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_NE_INT :
            STI(-2) = (STI(-2) != STI(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_NE_DOUBLE :
            STI(-2) = (STD(-2) != STD(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_NE_OBJECT :
            STI_WRITE(-2, STO(-2).data != STO(-1).data ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_NE_STRING :
            {
                std::basic_string<DVM_Char> str1(STO(-2).data->u.string.string ? STO(-2).data->u.string.string : L"");
                std::basic_string<DVM_Char> str2(STO(-1).data->u.string.string ? STO(-1).data->u.string.string : L"");
                
                STI_WRITE(-2, str1 != str2 ? DVM_TRUE : DVM_FALSE);
                pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_LOGICAL_AND :
            STI(-2) = (STI(-2) && STI(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_LOGICAL_OR :
            STI(-2) = (STI(-2) || STI(-1) ? DVM_TRUE : DVM_FALSE);
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_LOGICAL_NOT :
            STI(-1) = !STI(-1);
            pc++;
            break;

        case DVM_POP :
            pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_DUPLICATE :
            pVirtualMachine->stack.stack[pVirtualMachine->stack.stack_pointer] = pVirtualMachine->stack.stack[pVirtualMachine->stack.stack_pointer - 1];
            pVirtualMachine->stack.pointer_flags[pVirtualMachine->stack.stack_pointer] = pVirtualMachine->stack.pointer_flags[pVirtualMachine->stack.stack_pointer - 1];
            pVirtualMachine->stack.stack_pointer++;
            pc++;
            break;

        case DVM_DUPLICATE_OFFSET :
            {
                int offset = GET_2BYTE_INT(&pCode[pc + 1]);

                pVirtualMachine->stack.stack[pVirtualMachine->stack.stack_pointer] = pVirtualMachine->stack.stack[pVirtualMachine->stack.stack_pointer - 1 - offset];
                pVirtualMachine->stack.pointer_flags[pVirtualMachine->stack.stack_pointer] = pVirtualMachine->stack.pointer_flags[pVirtualMachine->stack.stack_pointer - 1 - offset];
                pVirtualMachine->stack.stack_pointer++;

                pc += 3;
            }
            break;

        case DVM_JUMP :
            pc = GET_2BYTE_INT(&pCode[pc + 1]);
            break;

        case DVM_JUMP_IF_TRUE :
            if (DVM_TRUE == STI(-1))
            {
                pc = GET_2BYTE_INT(&pCode[pc + 1]);
            }
            else
            {
                pc += 3;
            }
            pVirtualMachine->stack.stack_pointer--;
            break;

        case DVM_JUMP_IF_FALSE :
            if (DVM_FALSE == STI(-1))
            {
                pc = GET_2BYTE_INT(&pCode[pc + 1]);
            }
            else
            {
                pc += 3;
            }
            pVirtualMachine->stack.stack_pointer--;
            break;

        case DVM_PUSH_FUNCTION :
            STI_WRITE(0, GET_2BYTE_INT(&pCode[pc + 1]));
            pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_METHOD :
            {
                DVM_ObjectRef value = STO(-1);
                int index = GET_2BYTE_INT(&pCode[pc + 1]);

                CheckNullPointer(exe, pFunction, pc, &value);
                STI_WRITE(0, value.v_table->table[index].index);
                pVirtualMachine->stack.stack_pointer++;

                pc += 3;
            }
            break;

        case DVM_INVOKE :
            {
                int iFuncIndex = STI(-1);

                if (NATIVE_FUNCTION == pVirtualMachine->function[iFuncIndex]->kind)
                {
                    RestorePC(pVirtualMachine, ee, pFunction, pc);
                    InvokeNativeFunction(pVirtualMachine, pFunction, pVirtualMachine->function[iFuncIndex], pc, &pVirtualMachine->stack.stack_pointer, iBase);
                    pc++;
                }
                else
                {
                    InvokeDikSamFunction(pVirtualMachine, &pFunction, pVirtualMachine->function[iFuncIndex], &pCode, &iCodeSize, &pc, &pVirtualMachine->stack.stack_pointer, &iBase, &ee, &exe);
                }
            }
            break;

        case DVM_RETURN :
            if (DVM_TRUE == ReturnFunction(pVirtualMachine, &pFunction, &pCode, &iCodeSize, &pc, &iBase, &ee, &exe))
            {
                ret = pVirtualMachine->stack.stack[pVirtualMachine->stack.stack_pointer - 1];
                bReturn = true;
            }
            break;

        case DVM_NEW :
            {
                int index = GET_2BYTE_INT(&pCode[pc + 1]);

                STO_WRITE(0, m_Heap.CreateClassObject(pVirtualMachine, index));
                pVirtualMachine->stack.stack_pointer++;

                pc += 3;
            }
            break;

        case DVM_NEW_ARRAY :
            {
                int dim = pCode[pc + 1];
                DVM_TypeSpecifier *pType = &exe->type_specifier[GET_2BYTE_INT(&pCode[pc + 2])];

                RestorePC(pVirtualMachine, ee, pFunction, pc);
                DVM_ObjectRef array = CreateArray(pVirtualMachine, dim, pType);
                pVirtualMachine->stack.stack_pointer -= dim;
                STO_WRITE(0, array);

                pVirtualMachine->stack.stack_pointer++;
                pc += 4;
            }
            break;

        case DVM_NEW_ARRAY_LITERAL_INT :
            {
                int size = GET_2BYTE_INT(&pCode[pc + 1]);

                RestorePC(pVirtualMachine, ee, pFunction, pc);
                DVM_ObjectRef array = CreateArrayLiteralInt(pVirtualMachine, size);
                pVirtualMachine->stack.stack_pointer -= size;

                STO_WRITE(0, array);
                pVirtualMachine->stack.stack_pointer++;
                pc += 3;
            }
            break;

        case DVM_NEW_ARRAY_LITERAL_DOUBLE :
            {
                int size = GET_2BYTE_INT(&pCode[pc + 1]);

                RestorePC(pVirtualMachine, ee, pFunction, pc);
                DVM_ObjectRef array = CreateArrayLiteralDouble(pVirtualMachine, size);
                pVirtualMachine->stack.stack_pointer -= size;

                STO_WRITE(0, array);
                pVirtualMachine->stack.stack_pointer++;
                pc += 3;
            }
            break;

        case DVM_NEW_ARRAY_LITERAL_OBJECT :
            {
                int size = GET_2BYTE_INT(&pCode[pc + 1]);

                RestorePC(pVirtualMachine, ee, pFunction, pc);
                DVM_ObjectRef array = CreateArrayLiteralObject(pVirtualMachine, size);
                pVirtualMachine->stack.stack_pointer -= size;

                STO_WRITE(0, array);
                pVirtualMachine->stack.stack_pointer++;
                pc += 3;
            }
            break;

        case DVM_SUPER :
            {
                DVM_ObjectRef *pObj = &STO(-1);
                ExecClass *pThis = pObj->v_table->exec_class;

                pObj->v_table = pThis->super_class->class_table;

                pc++;
            }
            break;

        case DVM_INSTANCEOF :
            {
                DVM_ObjectRef *pObj = &STO(-1);
                int targetIndex = GET_2BYTE_INT(&pCode[pc + 1]);

                if (pObj->v_table->exec_class->class_index == targetIndex)
                {
                    STI_WRITE(-1, DVM_TRUE);
                }
                else
                {
                    STI_WRITE(-1, CheckInstanceOf(pVirtualMachine, pObj, targetIndex));
                }

                pc += 3;
            }
            break;

        default :
            DBG_assert(0, ("pCode[", pc, "]..", pCode[pc]));
        }
    }

    return ret;
}

void Execute::CheckNullPointer(DVM_Executable *pExecutable, Function *pFunction, int iPC, DVM_ObjectRef *pObj)
{
    if (nullptr == pObj->data)
    {
        m_Error.DVMError(pExecutable, pFunction, iPC, NULL_POINTER_ERR, MESSAGE_ARGUMENT_END);
    }
}

DVM_Boolean Execute::CheckInstanceOf(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef *pObj, int iTargetIdx, DVM_Boolean *pIsInterface, int *pInterfaceIdx)
{
    for (ExecClass *pos = pObj->v_table->exec_class->super_class; pos; pos = pos->super_class)
    {
        if (pos->class_index == iTargetIdx)
        {
            if (pIsInterface != nullptr)
                *pIsInterface = DVM_FALSE;

            return DVM_TRUE;
        }
    }

    for (int i = 0; i < pObj->v_table->exec_class->interface_count; i++)
    {
        if (pObj->v_table->exec_class->interface_[i]->class_index == iTargetIdx)
        {
            if (pIsInterface)
                *pIsInterface = DVM_TRUE;

            if (pInterfaceIdx)
                *pInterfaceIdx = i;

            return DVM_TRUE;
        }
    }

    return DVM_FALSE;
}

void Execute::CheckDownCast(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, Function *pFunction, int iPC, DVM_ObjectRef *pObject, int iTargetIdx, DVM_Boolean *pIsSameClass, DVM_Boolean *pIsInterface, int *pInterfaceIndex)
{
    if (pObject->v_table->exec_class->class_index == iTargetIdx)
    {
        *pIsSameClass = DVM_TRUE;
        return;
    }

    *pIsSameClass = DVM_FALSE;

    if (DVM_FALSE == CheckInstanceOf(pVirtualMachine, pObject, iTargetIdx, pIsInterface, pInterfaceIndex))
    {
        m_Error.DVMError(pExecutable, pFunction, iPC, CLASS_CAST_ERR,
            STRING_MESSAGE_ARGUMENT, "org", pObject->v_table->exec_class->name,
            STRING_MESSAGE_ARGUMENT, "target", pVirtualMachine->classes[iTargetIdx]->name,
            MESSAGE_ARGUMENT_END);
    }
}

void Execute::PushObject(DVM_VirtualMachine *pVirtualMachine, DVM_Value value)
{
    STO_WRITE(0, value.object);
    pVirtualMachine->stack.stack_pointer++;
}

DVM_Value Execute::PopObject(DVM_VirtualMachine *pVirtualMachine)
{
    DVM_Value value;

    value.object = STO(-1);
    pVirtualMachine->stack.stack_pointer--;

    return value;
}

void Execute::DisposeVTable(DVM_VTable *pVTable)
{
    for (int i = 0; i < pVTable->table_size; i++)
    {
        MEM_free(pVTable->table[i].name);
    }

    MEM_free(pVTable->table);
    MEM_free(pVTable);
}

int Execute::GetFieldIndexSub(ExecClass *pExecClass, char *lpstrFieldName, int *pSuperCount)
{
    if (pExecClass->super_class)
    {
        int index = GetFieldIndexSub(pExecClass->super_class, lpstrFieldName, pSuperCount);
        
        if (index != FIELD_NOT_FOUND)
        {
            return index;
        }
    }

    for (int i = 0; i < pExecClass->dvm_class->field_count; i++)
    {
        if (std::string(pExecClass->dvm_class->field[i].name) == lpstrFieldName)
        {
            return i + *pSuperCount;
        }
    }

    *pSuperCount += pExecClass->dvm_class->field_count;

    return FIELD_NOT_FOUND;
}

int Execute::GetFieldIndex(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef obj, char *lpstrFieldName)
{
    int iSuperCount = 0;

    return GetFieldIndexSub(obj.v_table->exec_class, lpstrFieldName, &iSuperCount);
}

inline bool Execute::IsPointerType(DVM_TypeSpecifier *pType)
{
    return (DVM_STRING_TYPE == pType->basic_type
        || DVM_CLASS_TYPE == pType->basic_type
        || DVM_NULL_TYPE == pType->basic_type
        || (pType->derive_count > 0 && DVM_ARRAY_DERIVE == pType->derive[0].tag));
}