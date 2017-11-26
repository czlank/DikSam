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

#define STI(sp)     ((m_pVirtualMachine)->stack.stack[(m_pVirtualMachine)->stack.stack_pointer + (sp)].int_value)
#define STD(sp)     ((m_pVirtualMachine)->stack.stack[(m_pVirtualMachine)->stack.stack_pointer + (sp)].double_value)
#define STO(sp)     ((m_pVirtualMachine)->stack.stack[(m_pVirtualMachine)->stack.stack_pointer + (sp)].object)

#define STI_I(sp)   ((m_pVirtualMachine)->stack.stack[(sp)].int_value)
#define STD_I(sp)   ((m_pVirtualMachine)->stack.stack[(sp)].double_value)
#define STO_I(sp)   ((m_pVirtualMachine)->stack.stack[(sp)].object)

#define STI_WRITE(sp, r) \
    ((m_pVirtualMachine)->stack.stack[(m_pVirtualMachine)->stack.stack_pointer + (sp)].int_value = r, \
    (m_pVirtualMachine)->stack.pointer_flags[(m_pVirtualMachine)->stack.stack_pointer + (sp)] = DVM_FALSE)
#define STD_WRITE(sp, r) \
    ((m_pVirtualMachine)->stack.stack[(m_pVirtualMachine)->stack.stack_pointer + (sp)].double_value = r, \
    (m_pVirtualMachine)->stack.pointer_flags[(m_pVirtualMachine)->stack.stack_pointer + (sp)] = DVM_FALSE)
#define STO_WRITE(sp, r) \
    ((m_pVirtualMachine)->stack.stack[(m_pVirtualMachine)->stack.stack_pointer + (sp)].object = r, \
    (m_pVirtualMachine)->stack.pointer_flags[(m_pVirtualMachine)->stack.stack_pointer + (sp)] = DVM_TRUE)

#define STI_WRITE_I(sp, r) \
    ((m_pVirtualMachine)->stack.stack[(sp)].int_value = r, \
    (m_pVirtualMachine)->stack.pointer_flags[(sp)] = DVM_FALSE)
#define STD_WRITE_I(sp, r) \
    ((m_pVirtualMachine)->stack.stack[(sp)].double_value = r, \
    (m_pVirtualMachine)->stack.pointer_flags[(sp)] = DVM_FALSE)
#define STO_WRITE_I(sp, r) \
    ((m_pVirtualMachine)->stack.stack[(sp)].object = r, \
    (m_pVirtualMachine)->stack.pointer_flags[(sp)] = DVM_TRUE)

Execute::Execute(Debug& debug, Memory& memory, Util& util, Error& error, Interface& interfaceRef)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Util(util)
    , m_Error(error)
    , m_Interface(interfaceRef)
    , m_GarbageCollect(debug, memory)
{
}

Execute::~Execute()
{
}

DVM_Value Execute::operator () (DVM_Executable* pExecutable)
{
    CreateVirtualMachine();

    AddExecutable(pExecutable);

    m_pVirtualMachine->current_executable = pExecutable;
    m_pVirtualMachine->current_function = nullptr;
    m_pVirtualMachine->pc = 0;

    ExpandStack(m_pVirtualMachine->executable->need_stack_size);
    DVM_Value ret = ExecuteCode(nullptr, m_pVirtualMachine->executable->code, m_pVirtualMachine->executable->code_size);

    DisposeVirtualMachine();

    return ret;
}

int Execute::ArrayGetInt(DVM_Object *pArray, int iIndex)
{
    CheckArray(pArray, iIndex, m_pVirtualMachine->current_executable, m_pVirtualMachine->current_function, m_pVirtualMachine->pc);

    return pArray->u.array.u.int_array[iIndex];
}

double Execute::ArrayGetDouble(DVM_Object *pArray, int iIndex)
{
    CheckArray(pArray, iIndex, m_pVirtualMachine->current_executable, m_pVirtualMachine->current_function, m_pVirtualMachine->pc);

    return pArray->u.array.u.double_array[iIndex];
}

DVM_Object* Execute::ArrayGetObject(DVM_Object *pArray, int iIndex)
{
    CheckArray(pArray, iIndex, m_pVirtualMachine->current_executable, m_pVirtualMachine->current_function, m_pVirtualMachine->pc);

    return pArray->u.array.u.object[iIndex];
}

void Execute::ArraySetInt(DVM_Object* pArray, int iIndex, int value)
{
    CheckArray(pArray, iIndex, m_pVirtualMachine->current_executable, m_pVirtualMachine->current_function, m_pVirtualMachine->pc);

    pArray->u.array.u.int_array[iIndex] = value;
}

void Execute::ArraySetDouble(DVM_Object* pArray, int iIndex, double value)
{
    CheckArray(pArray, iIndex, m_pVirtualMachine->current_executable, m_pVirtualMachine->current_function, m_pVirtualMachine->pc);

    pArray->u.array.u.double_array[iIndex] = value;
}

void Execute::ArraySetObject(DVM_Object* pArray, int iIndex, DVM_Object *value)
{
    CheckArray(pArray, iIndex, m_pVirtualMachine->current_executable, m_pVirtualMachine->current_function, m_pVirtualMachine->pc);

    pArray->u.array.u.object[iIndex] = value;
}

void Execute::CheckArray(DVM_Object *pArray, int iIndex, DVM_Executable *pExecutable, Function *pFunction, int iPC)
{
    if (nullptr == pArray)
    {
        m_Error.DVMError(pExecutable, pFunction, iPC, NULL_POINTER_ERR, MESSAGE_ARGUMENT_END);
    }

    if (iIndex < 0 || iIndex >= pArray->u.array.size)
    {
        m_Error.DVMError(pExecutable, pFunction, iPC,
            INDEX_OUT_OF_BOUNDS_ERR, INT_MESSAGE_ARGUMENT, "index", iIndex, INT_MESSAGE_ARGUMENT, "size", pArray->u.array.size,
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

    DVM_ObjectRef ret = m_GarbageCollect.CreateString(pVirtualMachine, str);

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

    *ppExe = pCallee->u.diksam_f.executable;
    DVM_Function *pCalleeFunction = &(*ppExe)->function[pCallee->u.diksam_f.index];

    ExpandStack(CALL_INFO_ALIGN_SIZE + pCalleeFunction->local_variable_count + (*ppExe)->function[pCallee->u.diksam_f.index].need_stack_size);

    CallInfo *pCallInfo = (CallInfo*)&m_pVirtualMachine->stack.stack[(*pSP - 1)];
    pCallInfo->caller = *ppCaller;
    pCallInfo->caller_address = *pPC;
    pCallInfo->base = *pBase;

    for (int i = 0; i < CALL_INFO_ALIGN_SIZE; i++)
    {
        m_pVirtualMachine->stack.pointer_flags[(*pSP - 1) + i] = DVM_FALSE;
    }

    *pBase = (*pSP - 1) - pCalleeFunction->parameter_count;
    *ppCaller = pCallee;

    InitializeLocalVariables(pCalleeFunction, (*pSP - 1) + CALL_INFO_ALIGN_SIZE);

    *pSP = (*pSP - 1);
    *pSP += CALL_INFO_ALIGN_SIZE + pCalleeFunction->local_variable_count;
    *pPC = 0;

    *ppCode = (*ppExe)->function[pCallee->u.diksam_f.index].code;
    *pCodeSize = (*ppExe)->function[pCallee->u.diksam_f.index].code_size;
}

void Execute::ReturnFunction(Function **ppFunction, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pSP, int *pBase, DVM_Executable **ppExe)
{
    DVM_Value returnValue = m_pVirtualMachine->stack.stack[(*pSP - 1)];
    DVM_Function *pCallee = &(*ppExe)->function[(*ppFunction)->u.diksam_f.index];
    CallInfo *pCallInfo = (CallInfo*)&m_pVirtualMachine->stack.stack[(*pSP - 1) - pCallee->local_variable_count - CALL_INFO_ALIGN_SIZE];

    if (pCallInfo->caller)
    {
        *ppExe = pCallInfo->caller->u.diksam_f.executable;

        DVM_Function *pCaller = &(*ppExe)->function[pCallInfo->caller->u.diksam_f.index];
        *ppCode = pCaller->code;
        *pCodeSize = pCaller->code_size;
    }
    else
    {
        *ppExe = m_pVirtualMachine->executable;
        *ppCode = m_pVirtualMachine->executable->code;
        *pCodeSize = m_pVirtualMachine->executable->code_size;
    }

    *ppFunction = pCallInfo->caller;
    *pPC = pCallInfo->caller_address + 1;
    *pBase = pCallInfo->base;
    *pSP -= pCallee->local_variable_count + CALL_INFO_ALIGN_SIZE + pCallee->parameter_count;

    m_pVirtualMachine->stack.stack[*pSP - 1] = returnValue;
}

DVM_Object* Execute::CreateArraySub(int iDim, int iDimIndex, DVM_TypeSpecifier *pType)
{
    DVM_Object *ret = nullptr;
    int size = STI(-iDim);

    if (iDimIndex == pType->derive_count - 1)
    {
        switch (pType->basic_type)
        {
        case DVM_BOOLEAN_TYPE :
        case DVM_INT_TYPE :
            ret = m_GarbageCollect.CreateArrayInt(m_pVirtualMachine, size);
            break;

        case DVM_DOUBLE_TYPE :
            ret = m_GarbageCollect.CreateArrayDouble(m_pVirtualMachine, size);
            break;

        case DVM_STRING_TYPE :
            ret = m_GarbageCollect.CreateArrayObject(m_pVirtualMachine, size);
            break;

        case DVM_NULL_TYPE :
        default :
            DBG_assert(0, ("pType->basic_type..", pType->basic_type));
        }
    }
    else if (DVM_FUNCTION_DERIVE == pType->derive[iDimIndex].tag)
    {
        ret = nullptr;
    }
    else
    {
        ret = m_GarbageCollect.CreateArrayObject(m_pVirtualMachine, size);

        if (iDimIndex < iDim - 1)
        {
            STO_WRITE(0, ret);
            m_pVirtualMachine->stack.stack_pointer++;

            for (int i = 0; i < size; i++) {
                DVM_Object *child = CreateArraySub(iDim, iDimIndex + 1, pType);
                ArraySetObject(ret, i, child);
            }

            m_pVirtualMachine->stack.stack_pointer--;
        }
    }

    return ret;
}

DVM_Object* Execute::CreateArray(int iDim, DVM_TypeSpecifier *pType)
{
    return CreateArraySub(iDim, 0, pType);
}

DVM_Object* Execute::CreateArrayLiteralInt(int iSize)
{
    DVM_Object *pArray = m_GarbageCollect.CreateArrayIntI(m_pVirtualMachine, iSize);

    for (int i = 0; i < iSize; i++)
    {
        pArray->u.array.u.int_array[i] = STI(-iSize + i);
    }

    return pArray;
}

DVM_Object* Execute::CreateArrayLiteralDouble(int iSize)
{
    DVM_Object *pArray = m_GarbageCollect.CreateArrayDoubleI(m_pVirtualMachine, iSize);

    for (int i = 0; i < iSize; i++)
    {
        pArray->u.array.u.double_array[i] = STD(-iSize + i);
    }

    return pArray;
}

DVM_Object* Execute::CreateArrayLiteralObject(int iSize)
{
    DVM_Object *pArray = m_GarbageCollect.CreateArrayObjectI(m_pVirtualMachine, iSize);

    for (int i = 0; i < iSize; i++)
    {
        pArray->u.array.u.object[i] = STO(-iSize + i);
    }

    return pArray;
}

void Execute::RestorePC(DVM_Executable *pExecutable, Function *pFunction, int iPC)
{
    m_pVirtualMachine->current_executable = pExecutable;
    m_pVirtualMachine->current_function = pFunction;
    m_pVirtualMachine->pc = iPC;
}

DVM_Value Execute::ExecuteCode(Function *pFunction, DVM_Byte *pCode, int iCodeSize)
{
    DVM_Executable *exe = m_pVirtualMachine->executable;
    int base = 0;

    for (int pc = m_pVirtualMachine->pc; pc < iCodeSize;)
    {
        switch (pCode[pc])
        {
        case DVM_PUSH_INT_1BYTE :
            STI_WRITE(0, pCode[pc + 1]);
            m_pVirtualMachine->stack.stack_pointer++;
            pc += 2;
            break;

        case DVM_PUSH_INT_2BYTE :
            STI_WRITE(0, GET_2BYTE_INT(&pCode[pc + 1]));
            m_pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_INT :
            STI_WRITE(0, exe->constant_pool[GET_2BYTE_INT(&pCode[pc + 1])].u.c_int);
            m_pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_DOUBLE_0 :
            STD_WRITE(0, 0.0);
            m_pVirtualMachine->stack.stack_pointer++;
            pc++;
            break;

        case DVM_PUSH_DOUBLE_1 :
            STD_WRITE(0, 1.0);
            m_pVirtualMachine->stack.stack_pointer++;
            pc++;
            break;

        case DVM_PUSH_DOUBLE :
            STD_WRITE(0, exe->constant_pool[GET_2BYTE_INT(&pCode[pc + 1])].u.c_double);
            m_pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_STRING :
            STO_WRITE(0, m_GarbageCollect.LiteralToString(m_pVirtualMachine, exe->constant_pool[GET_2BYTE_INT(&pCode[pc + 1])].u.c_string));
            m_pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_NULL :
            STO_WRITE(0, nullptr);
            m_pVirtualMachine->stack.stack_pointer++;
            pc++;
            break;

        case DVM_PUSH_STACK_INT :
            STI_WRITE(0, STI_I(base + GET_2BYTE_INT(&pCode[pc + 1])));
            m_pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_STACK_DOUBLE :
            STD_WRITE(0, STD_I(base + GET_2BYTE_INT(&pCode[pc + 1])));
            m_pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_STACK_OBJECT :
            STO_WRITE(0, STO_I(base + GET_2BYTE_INT(&pCode[pc + 1])));
            m_pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_POP_STACK_INT :
            STI_WRITE_I(base + GET_2BYTE_INT(&pCode[pc + 1]), STI(-1));
            m_pVirtualMachine->stack.stack_pointer--;
            pc += 3;
            break;

        case DVM_POP_STACK_DOUBLE :
            STD_WRITE_I(base + GET_2BYTE_INT(&pCode[pc + 1]), STD(-1));
            m_pVirtualMachine->stack.stack_pointer--;
            pc += 3;
            break;

        case DVM_POP_STACK_OBJECT :
            STO_WRITE_I(base + GET_2BYTE_INT(&pCode[pc + 1]), STO(-1));
            m_pVirtualMachine->stack.stack_pointer--;
            pc += 3;
            break;

        case DVM_PUSH_STATIC_INT :
            STI_WRITE(0, m_pVirtualMachine->static_v.variable[GET_2BYTE_INT(&pCode[pc + 1])].int_value);
            m_pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_STATIC_DOUBLE :
            STD_WRITE(0, m_pVirtualMachine->static_v.variable[GET_2BYTE_INT(&pCode[pc + 1])].double_value);
            m_pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_PUSH_STATIC_OBJECT :
            STO_WRITE(0, m_pVirtualMachine->static_v.variable[GET_2BYTE_INT(&pCode[pc + 1])].object);
            m_pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_POP_STATIC_INT :
            m_pVirtualMachine->static_v.variable[GET_2BYTE_INT(&pCode[pc + 1])].int_value = STI(-1);
            m_pVirtualMachine->stack.stack_pointer--;
            pc += 3;
            break;

        case DVM_POP_STATIC_DOUBLE :
            m_pVirtualMachine->static_v.variable[GET_2BYTE_INT(&pCode[pc + 1])].double_value = STD(-1);
            m_pVirtualMachine->stack.stack_pointer--;
            pc += 3;
            break;

        case DVM_POP_STATIC_OBJECT :
            m_pVirtualMachine->static_v.variable[GET_2BYTE_INT(&pCode[pc + 1])].object = STO(-1);
            m_pVirtualMachine->stack.stack_pointer--;
            pc += 3;
            break;

        case DVM_PUSH_ARRAY_INT :
            {
                DVM_Object* pArray = STO(-2);
                int index = STI(-1);

                RestorePC(exe, pFunction, pc);
                int value = ArrayGetInt(pArray, index);

                STI_WRITE(-2, value);
                m_pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_PUSH_ARRAY_DOUBLE :
            {
                DVM_Object *pArray = STO(-2);
                int index = STI(-1);

                RestorePC(exe, pFunction, pc);
                double value = ArrayGetDouble(pArray, index);

                STD_WRITE(-2, value);
                m_pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_PUSH_ARRAY_OBJECT :
            {
                DVM_Object *pArray = STO(-2);
                int index = STI(-1);

                RestorePC(exe, pFunction, pc);
                DVM_Object *value = ArrayGetObject(pArray, index);

                STO_WRITE(-2, value);
                m_pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_POP_ARRAY_INT :
            {
                int value = STI(-3);
                DVM_Object *pArray = STO(-2);
                int index = STI(-1);

                RestorePC(exe, pFunction, pc);
                ArraySetInt(pArray, index, value);

                m_pVirtualMachine->stack.stack_pointer -= 3;
                pc++;
            }
            break;

        case DVM_POP_ARRAY_DOUBLE :
            {
                double value = STD(-3);
                DVM_Object *pArray = STO(-2);
                int index = STI(-1);

                RestorePC(exe, pFunction, pc);
                ArraySetDouble(pArray, index, value);

                m_pVirtualMachine->stack.stack_pointer -= 3;
                pc++;
            }
            break;

        case DVM_POP_ARRAY_OBJECT :
            {
                DVM_Object *value = STO(-3);
                DVM_Object *pArray = STO(-2);
                int index = STI(-1);

                RestorePC(exe, pFunction, pc);
                ArraySetObject(pArray, index, value);

                m_pVirtualMachine->stack.stack_pointer -= 3;
                pc++;
            }
            break;

        case DVM_ADD_INT :
            STI(-2) = STI(-2) + STI(-1);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_ADD_DOUBLE :
            STD(-2) = STD(-2) + STD(-1);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_ADD_STRING :
            STO(-2) = ChainString(STO(-2), STO(-1));
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_SUB_INT :
            STI(-2) = STI(-2) - STI(-1);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_SUB_DOUBLE :
            STD(-2) = STD(-2) - STD(-1);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_MUL_INT :
            STI(-2) = STI(-2) * STI(-1);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_MUL_DOUBLE :
            STD(-2) = STD(-2) * STD(-1);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_DIV_INT :
            if (0 == STI(-1))
            {
                m_Error.DVMError(exe, pFunction, pc, DIVISION_BY_ZERO_ERR, MESSAGE_ARGUMENT_END);
            }

            STI(-2) = STI(-2) / STI(-1);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_DIV_DOUBLE :
            STD(-2) = STD(-2) / STD(-1);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_MOD_INT :
            STI(-2) = STI(-2) % STI(-1);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_MOD_DOUBLE :
            STD(-2) = std::fmod(STD(-2), STD(-1));
            m_pVirtualMachine->stack.stack_pointer--;
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
                STO_WRITE(-1, m_GarbageCollect.LiteralToString(m_pVirtualMachine, TRUE_STRING));
            }
            else
            {
                STO_WRITE(-1, m_GarbageCollect.LiteralToString(m_pVirtualMachine, FALSE_STRING));
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

                STO_WRITE(-1, m_GarbageCollect.CreateString(m_pVirtualMachine, str));
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

                STO_WRITE(-1, m_GarbageCollect.CreateString(m_pVirtualMachine, str));
                pc++;
            }
            break;

        case DVM_EQ_INT :
            STI(-2) = (STI(-2) == STI(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_EQ_DOUBLE :
            STI(-2) = (STD(-2) == STD(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_EQ_OBJECT :
            STI_WRITE(-2, STO(-2) == STO(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_EQ_STRING :
            {
                std::basic_string<DVM_Char> str1(STO(-2)->u.string.string ? STO(-2)->u.string.string : L"");
                std::basic_string<DVM_Char> str2(STO(-1)->u.string.string ? STO(-1)->u.string.string : L"");
                
                STI_WRITE(-2, str1 == str2 ? DVM_TRUE : DVM_FALSE);
                m_pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_GT_INT :
            STI(-2) = (STI(-2) > STI(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_GT_DOUBLE :
            STI(-2) = (STD(-2) > STD(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_GT_STRING :
            {
                std::basic_string<DVM_Char> str1(STO(-2)->u.string.string ? STO(-2)->u.string.string : L"");
                std::basic_string<DVM_Char> str2(STO(-1)->u.string.string ? STO(-1)->u.string.string : L"");
                
                STI_WRITE(-2, str1 > str2 ? DVM_TRUE : DVM_FALSE);
                m_pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_GE_INT :
            STI(-2) = (STI(-2) >= STI(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_GE_DOUBLE :
            STI(-2) = (STD(-2) >= STD(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_GE_STRING :
            {
                std::basic_string<DVM_Char> str1(STO(-2)->u.string.string ? STO(-2)->u.string.string : L"");
                std::basic_string<DVM_Char> str2(STO(-1)->u.string.string ? STO(-1)->u.string.string : L"");
                
                STI_WRITE(-2, str1 >= str2 ? DVM_TRUE : DVM_FALSE);
                m_pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_LT_INT :
            STI(-2) = (STI(-2) < STI(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_LT_DOUBLE :
            STI(-2) = (STD(-2) < STD(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_LT_STRING :
            {
                std::basic_string<DVM_Char> str1(STO(-2)->u.string.string ? STO(-2)->u.string.string : L"");
                std::basic_string<DVM_Char> str2(STO(-1)->u.string.string ? STO(-1)->u.string.string : L"");
                
                STI_WRITE(-2, str1 < str2 ? DVM_TRUE : DVM_FALSE);
                m_pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_LE_INT :
            STI(-2) = (STI(-2) <= STI(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_LE_DOUBLE :
            STI(-2) = (STD(-2) <= STD(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_LE_STRING :
            {
                std::basic_string<DVM_Char> str1(STO(-2)->u.string.string ? STO(-2)->u.string.string : L"");
                std::basic_string<DVM_Char> str2(STO(-1)->u.string.string ? STO(-1)->u.string.string : L"");
                
                STI_WRITE(-2, str1 <= str2 ? DVM_TRUE : DVM_FALSE);
                m_pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_NE_INT :
            STI(-2) = (STI(-2) != STI(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_NE_DOUBLE :
            STI(-2) = (STD(-2) != STD(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_NE_OBJECT :
            STI_WRITE(-2, STO(-2) != STO(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_NE_STRING :
            {
                std::basic_string<DVM_Char> str1(STO(-2)->u.string.string ? STO(-2)->u.string.string : L"");
                std::basic_string<DVM_Char> str2(STO(-1)->u.string.string ? STO(-1)->u.string.string : L"");
                
                STI_WRITE(-2, str1 != str2 ? DVM_TRUE : DVM_FALSE);
                m_pVirtualMachine->stack.stack_pointer--;
                pc++;
            }
            break;

        case DVM_LOGICAL_AND :
            STI(-2) = (STI(-2) && STI(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_LOGICAL_OR :
            STI(-2) = (STI(-2) || STI(-1) ? DVM_TRUE : DVM_FALSE);
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_LOGICAL_NOT :
            STI(-1) = !STI(-1);
            pc++;
            break;

        case DVM_POP :
            m_pVirtualMachine->stack.stack_pointer--;
            pc++;
            break;

        case DVM_DUPLICATE :
            m_pVirtualMachine->stack.stack[m_pVirtualMachine->stack.stack_pointer] = m_pVirtualMachine->stack.stack[m_pVirtualMachine->stack.stack_pointer - 1];
            m_pVirtualMachine->stack.stack_pointer++;
            pc++;
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
            m_pVirtualMachine->stack.stack_pointer--;
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
            m_pVirtualMachine->stack.stack_pointer--;
            break;

        case DVM_PUSH_FUNCTION :
            STI_WRITE(0, GET_2BYTE_INT(&pCode[pc + 1]));
            m_pVirtualMachine->stack.stack_pointer++;
            pc += 3;
            break;

        case DVM_INVOKE :
            {
                int iFuncIndex = STI(-1);

                if (NATIVE_FUNCTION == m_pVirtualMachine->function[iFuncIndex].kind)
                {
                    InvokeNativeFunction(&m_pVirtualMachine->function[iFuncIndex], &m_pVirtualMachine->stack.stack_pointer);
                    pc++;
                }
                else
                {
                    InvokeDikSamFunction(&pFunction, &m_pVirtualMachine->function[iFuncIndex], &pCode, &iCodeSize, &pc, &m_pVirtualMachine->stack.stack_pointer, &base, &exe);
                }
            }
            break;

        case DVM_RETURN :
            ReturnFunction(&pFunction, &pCode, &iCodeSize, &pc, &m_pVirtualMachine->stack.stack_pointer, &base, &exe);
            break;

        case DVM_NEW_ARRAY :
            {
                int dim = pCode[pc + 1];
                DVM_TypeSpecifier *pType = &exe->type_specifier[GET_2BYTE_INT(&pCode[pc + 2])];

                RestorePC(exe, pFunction, pc);
                DVM_Object *pArray = CreateArray(dim, pType);
                m_pVirtualMachine->stack.stack_pointer -= dim;
                STO_WRITE(0, pArray);

                m_pVirtualMachine->stack.stack_pointer++;
                pc += 4;
            }
            break;

        case DVM_NEW_ARRAY_LITERAL_INT :
            {
                int size = GET_2BYTE_INT(&pCode[pc + 1]);

                RestorePC(exe, pFunction, pc);
                DVM_Object *pArray = CreateArrayLiteralInt(size);
                m_pVirtualMachine->stack.stack_pointer -= size;

                STO_WRITE(0, pArray);
                m_pVirtualMachine->stack.stack_pointer++;
                pc += 3;
            }
            break;

        case DVM_NEW_ARRAY_LITERAL_DOUBLE :
            {
                int size = GET_2BYTE_INT(&pCode[pc + 1]);

                RestorePC(exe, pFunction, pc);
                DVM_Object *pArray = CreateArrayLiteralDouble(size);
                m_pVirtualMachine->stack.stack_pointer -= size;

                STO_WRITE(0, pArray);
                m_pVirtualMachine->stack.stack_pointer++;
                pc += 3;
            }
            break;

        case DVM_NEW_ARRAY_LITERAL_OBJECT :
            {
                int size = GET_2BYTE_INT(&pCode[pc + 1]);

                RestorePC(exe, pFunction, pc);
                DVM_Object *pArray = CreateArrayLiteralObject(size);
                m_pVirtualMachine->stack.stack_pointer -= size;

                STO_WRITE(0, pArray);
                m_pVirtualMachine->stack.stack_pointer++;
                pc += 3;
            }
            break;

        default :
            DBG_assert(0, ("pCode[pc]..", pCode[pc]));
        }
    }

    return {};
}

void Execute::DisposeVirtualMachine()
{
    m_pVirtualMachine->static_v.variable_count = 0;
    m_GarbageCollect.GC(m_pVirtualMachine);

    MEM_free(m_pVirtualMachine->stack.stack);
    MEM_free(m_pVirtualMachine->stack.pointer_flags);

    MEM_free(m_pVirtualMachine->static_v.variable);

    for (int i = 0; i < m_pVirtualMachine->function_count; i++)
    {
        MEM_free(m_pVirtualMachine->function[i].name);
    }

    MEM_free(m_pVirtualMachine->function);

    Dispose(m_Debug, m_Memory)(m_pVirtualMachine->executable);

    MEM_free(m_pVirtualMachine);

    m_pVirtualMachine = nullptr;
}

inline bool Execute::IsPointerType(DVM_TypeSpecifier *pType)
{
    return (DVM_STRING_TYPE == pType->basic_type
        || DVM_CLASS_TYPE == pType->basic_type
        || DVM_NULL_TYPE == pType->basic_type
        || (pType->derive_count > 0 && DVM_ARRAY_DERIVE == pType->derive[0].tag));
}