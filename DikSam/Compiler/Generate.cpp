#include "StdAfx.h"
#include "Generate.h"
#include "Debug.h"
#include "Memory.h"
#include "Error.h"

Generate::Generate(Debug& debug, Memory& memory, Error& error)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Error(error)
{

}

Generate::~Generate()
{

}

DVM_Executable* Generate::operator () (DKC_Compiler *pCompiler)
{
    return nullptr;
}