#pragma once

#include "DikSamc.h"

class Debug;
class Memory;
class Storage;
class StringLiteral;

class Interface
{
#define ERROR_DBG_Panic(arg)                    (m_Debug.Panic(__FILE__, __LINE__, arg))
#define INTERFACE_STORAGE_OPEN(size)            (m_Storage.Open(__FILE__, __LINE__, size))
#define INTERFACE_STORAGE_MALLOC(storage, size) (m_Storage.Malloc(__FILE__, __LINE__, storage, size))

public:
    Interface(Debug& debug, Memory& memory, Storage& storage, StringLiteral& stringliteral);
    ~Interface();

    DKC_Compiler* GetCompiler() { return m_pCompiler; }
    void Compile(FILE *pFile);
    void Compile(char **ppLines);
    void DisposeCompiler();

private:
    void CreateCompiler();
    DVM_Executable* DoCompile(DKC_Compiler *pCompiler);

private:
    Debug           &m_Debug;
    Memory          &m_Memory;
    Storage         &m_Storage;
    StringLiteral   &m_StringLiteral;
    DKC_Compiler    *m_pCompiler;
    DVM_Executable  *m_pExe;
};