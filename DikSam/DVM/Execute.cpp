#include "StdAfx.h"
#include "Execute.h"
#include "Debug.h"
#include "Memory.h"
#include "Error.h"
#include "DVM_pri.h"
#include "GarbageCollect.h"
#include "Dispose.h"

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
    DisposeVirtualMachine();    // --> DVM_dispose_virtual_machine
}

DVM_Value Execute::operator () (DVM_Executable* pExecutable)
{
    CreateVirtualMachine();

    AddExecutable(m_pVirtualMachine, m_pExecutable);

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
