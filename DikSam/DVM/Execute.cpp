#include "StdAfx.h"
#include "Execute.h"
#include "Debug.h"
#include "Memory.h"
#include "Error.h"
#include "DVM_pri.h"

Execute::Execute(Debug& debug, Memory& memory, Error& error)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Error(error)
    , m_pExecutable(nullptr)
    , m_pVirtualMachine(nullptr)
{
    CreateVirtualMachine();     // --> DVM_create_virtual_machine
}

Execute::~Execute()
{
    DisposeVirtualMachine();    // --> DVM_dispose_virtual_machine
}

DVM_Value Execute::operator () (DVM_Executable* pExecutable)
{
    AddExecutable(m_pVirtualMachine, m_pExecutable);

    return DVM_Value{};
}

void Execute::AddExecutable(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable)
{

}

void Execute::CreateVirtualMachine()
{

}

void Execute::DisposeVirtualMachine()
{
    DisposeExecutable(m_pVirtualMachine->executable);
}

void Execute::DisposeExecutable(DVM_Executable *pExecutable)
{

}