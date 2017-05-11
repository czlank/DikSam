#pragma once

#include "DikSamc.h"

class Debug;
class Memory;
class Storage;
class Util;
class Error;
class StringLiteral;
class Create;
class Execute;

class Interface
{
#define ERROR_DBG_Panic(arg)                    (m_Debug.Panic(__FILE__, __LINE__, arg))
#define INTERFACE_STORAGE_OPEN(size)            (m_Storage.Open(__FILE__, __LINE__, size))
#define INTERFACE_STORAGE_MALLOC(storage, size) (m_Storage.Malloc(__FILE__, __LINE__, storage, size))

public:
    Interface(Debug& debug, Memory& memory, Storage& storage, Util& util, Error& error, StringLiteral& stringliteral, Create& create, int iThreadIndex);
    ~Interface();

    DKC_Compiler* GetCompiler() { return m_pCompiler; }

    void RunScript(FILE *pFile);
    void RunScript(char **ppLines);

private:
    DVM_Executable* Compile(FILE *pFile);
    DVM_Executable* Compile(char **ppLines);
    DVM_Executable* DoCompile(DKC_Compiler *pCompiler, char **ppLines);

    void ResetCompiler();
    void CreateCompiler();
    void DisposeCompiler();

private:
    Debug                   &m_Debug;
    Memory                  &m_Memory;
    Storage                 &m_Storage;
    Util                    &m_Util;
    Error                   &m_Error;
    StringLiteral           &m_StringLiteral;
    Create                  &m_Create;
    
    DKC_Compiler            *m_pCompiler;
    int                     m_iThreadIndex;

    std::ofstream           m_MemoryDump;

    static std::mutex   m_Mutex;
};