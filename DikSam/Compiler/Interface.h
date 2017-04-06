#pragma once

#include "DikSamc.h"

class Debug;
class Memory;
class Storage;
class Util;
class Error;
class StringLiteral;
class Create;

class Interface
{
#define ERROR_DBG_Panic(arg)                    (m_Debug.Panic(__FILE__, __LINE__, arg))
#define INTERFACE_STORAGE_OPEN(size)            (m_Storage.Open(__FILE__, __LINE__, size))
#define INTERFACE_STORAGE_MALLOC(storage, size) (m_Storage.Malloc(__FILE__, __LINE__, storage, size))

public:
    Interface(Debug& debug, Memory& memory, Storage& storage, Util& util, Error& error, StringLiteral& stringliteral, Create& create, int iThreadIndex);
    ~Interface();

    DKC_Compiler* GetCompiler() { return m_pCompiler; }
    void Compile(FILE *pFile);
    void Compile(char **ppLines);

private:
    void ResetCompiler();
    void CreateCompiler();
    void DisposeCompiler();
    DVM_Executable* DoCompile(DKC_Compiler *pCompiler, char **ppLines);

private:
    Debug               &m_Debug;
    Memory              &m_Memory;
    Storage             &m_Storage;
    Util                &m_Util;
    Error               &m_Error;
    StringLiteral       &m_StringLiteral;
    Create              &m_Create;
    DKC_Compiler        *m_pCompiler;
    DVM_Executable      *m_pExe;
    int                 m_iThreadIndex;

    static std::mutex   m_Mutex;
};