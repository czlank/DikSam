#include "StdAfx.h"
#include "Execute.h"
#include "Debug.h"
#include "Memory.h"
#include "Error.h"
#include "DVM_pri.h"
#include "GarbageCollect.h"
#include "Dispose.h"
#include "OpcodeInfo.h"

Execute::Execute(Debug& debug, Memory& memory, Error& error)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Error(error)
    , m_pExecutable(nullptr)
    , m_pVirtualMachine(nullptr)
{
}

Execute::~Execute()
{
}

DVM_Value Execute::operator () (DVM_Executable* pExecutable)
{
    CreateVirtualMachine();

    AddExecutable(m_pVirtualMachine, m_pExecutable);

    // ExecuteCode();

    DisposeVirtualMachine();
    return DVM_Value{};
}

void Execute::AddExecutable(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable)
{
    pVirtualMachine->executable = pExecutable;

    AddFunctions(pVirtualMachine, pExecutable);

    ConvertCode(pVirtualMachine, pExecutable, pExecutable->code, pExecutable->code_size, nullptr);

    for (int i = 0; i < pExecutable->function_count; i++)
    {
        DVM_Function *pFunction = &pExecutable->function[i];
        ConvertCode(pVirtualMachine, pExecutable, pFunction->code, pFunction->code_size, pFunction);
    }

    AddStaticVariables(pVirtualMachine, pExecutable);
}

void Execute::AddFunctions(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable)
{
    int iSrcIdx, iDestIdx;
    int iFunctionCount = 0;

    for (iSrcIdx = 0; iSrcIdx < pExecutable->function_count; iSrcIdx++)
    {
        if (DVM_TRUE == pExecutable->function[iSrcIdx].is_implemented)
        {
            for (iDestIdx = 0; iDestIdx < pVirtualMachine->function_count; iDestIdx++)
            {
                if (std::string(pVirtualMachine->function[iDestIdx].name) == pExecutable->function[iSrcIdx].name)
                {
                    m_Error.DVMError(nullptr, nullptr, NO_LINE_NUMBER_PC,
                        FUNCTION_MULTIPLE_DEFINE_ERR, STRING_MESSAGE_ARGUMENT, "name", pVirtualMachine->function[iDestIdx].name,
                        MESSAGE_ARGUMENT_END);
                }
            }

            iFunctionCount++;
        }
    }

    pVirtualMachine->function = (Function*)EXECUTE_MEM_Realloc(pVirtualMachine->function, sizeof(Function) * (pVirtualMachine->function_count + iFunctionCount));

    for (iSrcIdx = 0, iDestIdx = pVirtualMachine->function_count; iSrcIdx < pExecutable->function_count; iSrcIdx++)
    {
        if (DVM_FALSE == pExecutable->function[iSrcIdx].is_implemented)
            continue;

        pVirtualMachine->function[iDestIdx].name = EXECUTE_MEM_StrDUP(pExecutable->function[iSrcIdx].name);
        pVirtualMachine->function[iDestIdx].u.diksam_f.executable = pExecutable;
        pVirtualMachine->function[iDestIdx].u.diksam_f.index = iSrcIdx;

        iDestIdx++;
    }

    pVirtualMachine->function_count += iFunctionCount;
}

int Execute::SearchFunction(DVM_VirtualMachine *pVirtualMachine, char *lpstrName)
{
    for (int i = 0; i < pVirtualMachine->function_count; i++)
    {
        if (std::string(pVirtualMachine->function[i].name) == lpstrName)
        {
            return i;
        }
    }

    m_Error.DVMError(nullptr, nullptr, NO_LINE_NUMBER_PC,
        FUNCTION_NOT_FOUND_ERR, STRING_MESSAGE_ARGUMENT, "name", lpstrName,
        MESSAGE_ARGUMENT_END);

    return 0;
}

void Execute::ConvertCode(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, DVM_Byte *pCode, int iCodeSize, DVM_Function *pFunction)
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
            unsigned int iFunctionIdx = SearchFunction(pVirtualMachine, pExecutable->function[iIdxInExe].name);
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

void Execute::InitializeValue(DVM_VirtualMachine *pVirtualMachine, DVM_BasicType enType, DVM_Value *pValue)
{
    GarbageCollect garbageCollect(m_Debug, m_Memory);

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
        pValue->object = garbageCollect.LiteralToString(pVirtualMachine, L"");
        break;

    default :
        EXECUTE_DBG_Assert(0, ("enType..", enType));
    }
}

void Execute::AddStaticVariables(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable)
{
    pVirtualMachine->static_v.variable = (DVM_Value*)EXECUTE_MEM_Malloc(sizeof(DVM_Value) * pExecutable->global_variable_count);
    pVirtualMachine->static_v.variable_count = pExecutable->global_variable_count;

    for (int i = 0; i < pExecutable->global_variable_count; i++)
    {
        if (DVM_STRING_TYPE == pExecutable->global_variable[i].type->basic_type)
        {
            pVirtualMachine->static_v.variable[i].object = nullptr;
        }
    }

    for (int i = 0; i < pExecutable->global_variable_count; i++)
    {
        InitializeValue(pVirtualMachine, pExecutable->global_variable[i].type->basic_type, &pVirtualMachine->static_v.variable[i]);
    }
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
}
