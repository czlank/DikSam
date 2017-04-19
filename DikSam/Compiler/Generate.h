#pragma once

#include "DikSamc.h"

class Debug;
class Memory;
class Error;

class Generate
{
public:
    Generate(Debug& debug, Memory& memory, Error& error);
    ~Generate();

    DVM_Executable* operator () (DKC_Compiler *pCompiler);

private:


private:
    Debug   &m_Debug;
    Memory  &m_Memory;
    Error   &m_Error;
};