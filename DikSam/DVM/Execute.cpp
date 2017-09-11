#include "StdAfx.h"
#include "Execute.h"
#include "Debug.h"
#include "Memory.h"
#include "Error.h"
#include "DVM_pri.h"
#include "Dispose.h"
#include "OpcodeInfo.h"

Execute::Execute(Debug& debug, Memory& memory, Error& error)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Error(error)
    , m_Native(debug, memory)
    , m_GarbageCollect(debug, memory)
    , m_pVirtualMachine(nullptr)
{
}

Execute::~Execute()
{
}

DVM_Value Execute::operator () (DVM_Executable* pExecutable)
{
    CreateVirtualMachine();

    m_Native.AddNativeFunctions(m_pVirtualMachine);
    AddExecutable(pExecutable);

    m_pVirtualMachine->pc = 0;
    ExpandStack(m_pVirtualMachine->executable->need_stack_size);
    DVM_Value ret = ExecuteCode(nullptr, m_pVirtualMachine->executable->code, m_pVirtualMachine->executable->code_size);

    DisposeVirtualMachine();

    return ret;
}

void Execute::AddExecutable(DVM_Executable *pExecutable)
{
    m_pVirtualMachine->executable = pExecutable;

    AddFunctions(pExecutable);

    ConvertCode(pExecutable, pExecutable->code, pExecutable->code_size, nullptr);

    for (int i = 0; i < pExecutable->function_count; i++)
    {
        DVM_Function *pFunction = &pExecutable->function[i];
        ConvertCode(pExecutable, pFunction->code, pFunction->code_size, pFunction);
    }

    AddStaticVariables(pExecutable);
}

void Execute::AddFunctions(DVM_Executable *pExecutable)
{
    int iSrcIdx, iDestIdx;
    int iFunctionCount = 0;

    for (iSrcIdx = 0; iSrcIdx < pExecutable->function_count; iSrcIdx++)
    {
        if (DVM_TRUE == pExecutable->function[iSrcIdx].is_implemented)
        {
            for (iDestIdx = 0; iDestIdx < m_pVirtualMachine->function_count; iDestIdx++)
            {
                if (std::string(m_pVirtualMachine->function[iDestIdx].name) == pExecutable->function[iSrcIdx].name)
                {
                    m_Error.DVMError(nullptr, nullptr, NO_LINE_NUMBER_PC,
                        FUNCTION_MULTIPLE_DEFINE_ERR, STRING_MESSAGE_ARGUMENT, "name", m_pVirtualMachine->function[iDestIdx].name,
                        MESSAGE_ARGUMENT_END);
                }
            }

            iFunctionCount++;
        }
    }

    m_pVirtualMachine->function = (Function*)EXECUTE_MEM_Realloc(m_pVirtualMachine->function, sizeof(Function) * (m_pVirtualMachine->function_count + iFunctionCount));

    for (iSrcIdx = 0, iDestIdx = m_pVirtualMachine->function_count; iSrcIdx < pExecutable->function_count; iSrcIdx++)
    {
        if (DVM_FALSE == pExecutable->function[iSrcIdx].is_implemented)
            continue;

        m_pVirtualMachine->function[iDestIdx].name = EXECUTE_MEM_StrDUP(pExecutable->function[iSrcIdx].name);
        m_pVirtualMachine->function[iDestIdx].kind = DIKSAM_FUNCTION;
        m_pVirtualMachine->function[iDestIdx].u.diksam_f.executable = pExecutable;
        m_pVirtualMachine->function[iDestIdx].u.diksam_f.index = iSrcIdx;

        iDestIdx++;
    }

    m_pVirtualMachine->function_count += iFunctionCount;
}

int Execute::SearchFunction(char *lpstrName)
{
    for (int i = 0; i < m_pVirtualMachine->function_count; i++)
    {
        if (std::string(m_pVirtualMachine->function[i].name) == lpstrName)
        {
            return i;
        }
    }

    m_Error.DVMError(nullptr, nullptr, NO_LINE_NUMBER_PC,
        FUNCTION_NOT_FOUND_ERR, STRING_MESSAGE_ARGUMENT, "name", lpstrName,
        MESSAGE_ARGUMENT_END);

    return 0;
}

void Execute::ConvertCode(DVM_Executable *pExecutable, DVM_Byte *pCode, int iCodeSize, DVM_Function *pFunction)
{
    for (int i = 0; i < iCodeSize; i++)
    {
        if (DVM_PUSH_STACK_INT == pCode[i]
            || DVM_PUSH_STACK_DOUBLE == pCode[i]
            || DVM_PUSH_STACK_OBJECT == pCode[i]
            || DVM_POP_STACK_INT == pCode[i]
            || DVM_POP_STACK_DOUBLE == pCode[i]
            || DVM_POP_STACK_OBJECT == pCode[i])
        {
            EXECUTE_DBG_Assert(pFunction != nullptr, ("pFunction == nullptr"));

            int iSrcIdx = GET_2BYTE_INT(&pCode[i + 1]);
            unsigned int iDestIdx;

            if (iSrcIdx >= pFunction->parameter_count)
            {
                iDestIdx = iSrcIdx + CALL_INFO_ALIGN_SIZE;
            }
            else
            {
                iDestIdx = iSrcIdx;
            }

            SET_2BYTE_INT(&pCode[i + 1], iDestIdx);
        }
        else if (DVM_PUSH_FUNCTION == pCode[i])
        {
            int iIdxInExe = GET_2BYTE_INT(&pCode[i + 1]);
            unsigned int iFunctionIdx = SearchFunction(pExecutable->function[iIdxInExe].name);
            SET_2BYTE_INT(&pCode[i + 1], iFunctionIdx);
        }

        OpcodeInfo &Info = DVMOpcodeInfo::Opcode()[pCode[i]];

        for (int idx = 0; Info.parameter[idx] != '\0'; idx++)
        {
            switch (Info.parameter[idx])
            {
            case 'b' :
                i++;
                break;

            case 's' :
            case 'p' :
                i += 2;
                break;

            default :
                EXECUTE_DBG_Assert(0, ("param..", Info.parameter, ", idx..", idx));
            }
        }
    }
}

void Execute::InitializeValue(DVM_BasicType enType, DVM_Value *pValue)
{
    switch (enType)
    {
    case DVM_BOOLEAN_TYPE :
    case DVM_INT_TYPE :
        pValue->int_value = 0;
        break;

    case DVM_DOUBLE_TYPE :
        pValue->double_value = 0.0;
        break;

    case DVM_STRING_TYPE :
        pValue->object = m_GarbageCollect.LiteralToString(m_pVirtualMachine, L"");
        break;

    default :
        EXECUTE_DBG_Assert(0, ("enType..", enType));
    }
}

void Execute::AddStaticVariables(DVM_Executable *pExecutable)
{
    m_pVirtualMachine->static_v.variable = (DVM_Value*)EXECUTE_MEM_Malloc(sizeof(DVM_Value) * pExecutable->global_variable_count);
    m_pVirtualMachine->static_v.variable_count = pExecutable->global_variable_count;

    for (int i = 0; i < pExecutable->global_variable_count; i++)
    {
        if (DVM_STRING_TYPE == pExecutable->global_variable[i].type->basic_type)
        {
            m_pVirtualMachine->static_v.variable[i].object = nullptr;
        }
    }

    for (int i = 0; i < pExecutable->global_variable_count; i++)
    {
        InitializeValue(pExecutable->global_variable[i].type->basic_type, &m_pVirtualMachine->static_v.variable[i]);
    }
}

DVM_Object* Execute::ChainString(DVM_Object *str1, DVM_Object *str2)
{
    int iLen = str1->u.string.string ? std::wstring(str1->u.string.string).length() : 0;
    iLen += str2->u.string.string ? std::wstring(str2->u.string.string).length() : 0;
    DVM_Char *str = (DVM_Char*)EXECUTE_MEM_Malloc(sizeof(DVM_Char) * (iLen + 1));

    int i = 0;
    int iLen1 = str1->u.string.string ? std::wstring(str1->u.string.string).length() : 0;
    int iLen2 = str1->u.string.string ? std::wstring(str2->u.string.string).length() : 0;

    for (i = 0; i < iLen1; i++)
    {
        str[i] = str1->u.string.string[i];
    }

    for (i = iLen1; i < iLen1 + iLen2; i++)
    {
        str[i] = str2->u.string.string[i - iLen1];
    }

    str[i] = 0;

    DVM_Object *ret = m_GarbageCollect.CreateString(m_pVirtualMachine, str);

    return ret;
}

void Execute::InitializeLocalVariables(DVM_Function *pFunction, int iFromSP)
{
    for (int i = 0, idx = iFromSP; i < pFunction->local_variable_count; i++, idx++)
    {
        m_pVirtualMachine->stack.pointer_flags[i] = DVM_FALSE;
    }

    for (int i = 0, idx = iFromSP; i < pFunction->local_variable_count; i++, idx++)
    {
        InitializeValue(pFunction->local_variable[i].type->basic_type, &m_pVirtualMachine->stack.stack[idx]);

        if (DVM_STRING_TYPE == pFunction->local_variable[i].type->basic_type)
        {
            m_pVirtualMachine->stack.pointer_flags[i] = DVM_TRUE;
        }
    }
}

void Execute::ExpandStack(int iNeedStackSize)
{
    int iRest = m_pVirtualMachine->stack.alloc_size - m_pVirtualMachine->stack.stack_pointer;

    if (iRest > iNeedStackSize)
        return;

    int iRevalueUp = ((iRest / STACK_ALLOC_SIZE) + 1) * STACK_ALLOC_SIZE;

    m_pVirtualMachine->stack.alloc_size += iRevalueUp;
    m_pVirtualMachine->stack.stack = (DVM_Value*)EXECUTE_MEM_Realloc(m_pVirtualMachine->stack.stack, sizeof(DVM_Value) * m_pVirtualMachine->stack.alloc_size);
    m_pVirtualMachine->stack.pointer_flags = (DVM_Boolean*)EXECUTE_MEM_Realloc(m_pVirtualMachine->stack.pointer_flags, sizeof(DVM_Boolean) * m_pVirtualMachine->stack.alloc_size);
}

void Execute::InvokeNativeFunction(Function *pFunction, int *pSP)
{
    EXECUTE_DBG_Assert(NATIVE_FUNCTION == pFunction->kind, ("pFunction->kind..", pFunction->kind));

    DVM_Value *stack = m_pVirtualMachine->stack.stack;
    int sp = *pSP;
    DVM_Value ret = pFunction->u.native_f.proc(pFunction->u.native_f.pThis,
        m_pVirtualMachine,
        pFunction->u.native_f.arg_count,
        &stack[sp - pFunction->u.native_f.arg_count - 1]);

    stack[sp - pFunction->u.native_f.arg_count - 1] = ret;
    *pSP = sp - (pFunction->u.native_f.arg_count);
}

void Execute::InvokeDikSamFunction(Function **ppCaller, Function *pCallee, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pSP, int *pBase, DVM_Executable **ppExe)
{
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

DVM_Value Execute::ExecuteCode(Function *pFunction, DVM_Byte *pCode, int iCodeSize)
{
    DVM_Value *stack = m_pVirtualMachine->stack.stack;
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

                DVM_Char *str = (DVM_Char*)EXECUTE_MEM_Malloc(sizeof(DVM_Char) * (wstr.length() + 1));
                
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

                DVM_Char *str = (DVM_Char*)EXECUTE_MEM_Malloc(sizeof(DVM_Char) * (wstr.length() + 1));
                
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

        case DVM_NE_DOUBLE:
            STI(-2) = (STD(-2) != STD(-1) ? DVM_TRUE : DVM_FALSE);
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
            stack[m_pVirtualMachine->stack.stack_pointer] = stack[m_pVirtualMachine->stack.stack_pointer - 1];
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

        default :
            EXECUTE_DBG_Assert(0, ("pCode[pc]..", pCode[pc]));
        }
    }

    return {};
}

void Execute::CreateVirtualMachine()
{
    m_pVirtualMachine = (DVM_VirtualMachine*)EXECUTE_MEM_Malloc(sizeof(DVM_VirtualMachine));
    
    m_pVirtualMachine->stack.alloc_size = STACK_ALLOC_SIZE;
    m_pVirtualMachine->stack.stack = (DVM_Value*)EXECUTE_MEM_Malloc(sizeof(DVM_Value) * STACK_ALLOC_SIZE);
    m_pVirtualMachine->stack.pointer_flags = (DVM_Boolean*)EXECUTE_MEM_Malloc(sizeof(DVM_Boolean) * STACK_ALLOC_SIZE);
    m_pVirtualMachine->stack.stack_pointer = 0;

    m_pVirtualMachine->heap.current_heap_size = 0;
    m_pVirtualMachine->heap.header = nullptr;
    m_pVirtualMachine->heap.current_threshold = HEAP_THRESHOLD_SIZE;

    m_pVirtualMachine->function = nullptr;
    m_pVirtualMachine->function_count = 0;

    m_pVirtualMachine->executable = nullptr;
}

void Execute::DisposeVirtualMachine()
{
    m_pVirtualMachine->static_v.variable_count = 0;
    m_GarbageCollect.GC(m_pVirtualMachine);

    EXECUTE_MEM_Free(m_pVirtualMachine->stack.stack);
    EXECUTE_MEM_Free(m_pVirtualMachine->stack.pointer_flags);

    EXECUTE_MEM_Free(m_pVirtualMachine->static_v.variable);

    for (int i = 0; i < m_pVirtualMachine->function_count; i++)
    {
        EXECUTE_MEM_Free(m_pVirtualMachine->function[i].name);
    }

    EXECUTE_MEM_Free(m_pVirtualMachine->function);

    Dispose(m_Debug, m_Memory)(m_pVirtualMachine->executable);

    EXECUTE_MEM_Free(m_pVirtualMachine);

    m_pVirtualMachine = nullptr;
}
