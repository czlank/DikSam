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
    , m_Native(memory)
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

    // ExecuteCode();

    DisposeVirtualMachine();

    return DVM_Value{};
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
            || DVM_PUSH_STACK_STRING == pCode[i]
            || DVM_POP_STACK_INT == pCode[i]
            || DVM_POP_STACK_DOUBLE == pCode[i]
            || DVM_POP_STACK_STRING == pCode[i])
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
    int iLen = str1->u.string.string ? std::wstring(str1->u.string.string).length() : 0
        + str2->u.string.string ? std::wstring(str2->u.string.string).length() : 0;
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
    DVM_Value ret = pFunction->u.native_f.proc(m_pVirtualMachine,
        pFunction->u.native_f.arg_count,
        &stack[sp - pFunction->u.native_f.arg_count - 1]);

    stack[sp - pFunction->u.native_f.arg_count - 1] = ret;
    *pSP = sp - (pFunction->u.native_f.arg_count);
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

    GarbageCollect garbageCollect(m_Debug, m_Memory);
    garbageCollect.GC(m_pVirtualMachine);

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
