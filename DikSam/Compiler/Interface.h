#pragma once

#include "DikSamc.h"
#include "DVM_dev.h"

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
public:
    Interface(Debug& debug, Memory& memory, Storage& storage, Util& util, Error& error, StringLiteral& stringliteral, Create& create, int iThreadIndex);
    ~Interface();

    DKC_Compiler* GetCurrentCompiler() { return m_pCompiler; }

    void RunScript(FILE *pFile, const char *lpstrPath);
    void RunScript(char **ppLines, const char *lpstrPath);

    SearchFileStatus DynamicCompile(DKC_Compiler *pCompiler, char *lpstrPackageName, DVM_ExecutableList *pExecutableList, DVM_ExecutableItem **ppAddTop, char *lpstrSearchFile);

private:
    typedef struct
    {
        char            *name;
        DVM_BasicType    type;
    } BuiltInMethodParameter;

    typedef struct
    {
        char                   *name;
        DVM_BasicType           return_type;
        BuiltInMethodParameter *parameter;
        int                     parameter_count;
    } BuiltInMethod;

private:
    DVM_ExecutableList* Compile(DKC_Compiler *pCompiler, FILE *pFile, const char *lpstrPath);
    DVM_ExecutableList* Compile(DKC_Compiler *pCompiler, char **ppLines, const char *lpstrPath);
    DVM_Executable* DoCompile(DKC_Compiler *pCompiler, char **ppLines, DVM_ExecutableList *pExecutableList, const char *lpstrPath, bool isRequired);

    void ResetCompiler();
    DKC_Compiler* CreateCompiler();
    void DisposeCompiler(DKC_Compiler *pCompiler);

    FunctionDefinition* CreateBuiltInMethod(BuiltInMethod *pMethod, int iMethodCount);
    void SetPathToCompiler(DKC_Compiler *pCompiler, const char *lpstrPath);
    void DisposeCompilerList();
    DKC_Compiler* SearchCompiler(CompilerList *pList, PackageName *pPackageName);
    CompilerList* AddCompilerToList(CompilerList *pList, DKC_Compiler *pCompilter);
    void GetRequireInput(RequireList *pReqList, char *lpstrFoundPath, SourceInput *pSourceInput);
    bool AddExeToList(DVM_Executable *pExecutable, DVM_ExecutableList *pList);
    void MakeSearchPath(int iLineNumber, PackageName *pPackageName, char *pBuf);
    void MakeSearchPathImpl(char *lpstrPackageName, char *pBuf);
    PackageName* CreateOnePackageName(DKC_Compiler *pCompiler, char *str, int iStartIdx, int iToIdx);
    PackageName* StringToPackageName(DKC_Compiler *pCompiler, char *str);
    SearchFileStatus GetDynamicLoadInput(char *lpstrPackageName, char *lpstrFoundPath, char *lpstrSearchFile, SourceInput *pSourceInput);
    CompilerList* TraversalCompiler(CompilerList *pList, DKC_Compiler *pCompiler);

private:
    Debug                          &m_Debug;
    Memory                         &m_Memory;
    Storage                        &m_Storage;
    Util                           &m_Util;
    Error                          &m_Error;
    StringLiteral                  &m_StringLiteral;
    Create                         &m_Create;
    
    DKC_Compiler                   *m_pCompiler;
    CompilerList                   *m_pCompilerList;

    int                             m_iThreadIndex;

    std::ofstream                   m_MemoryDump;

    static std::mutex               m_Mutex;

    static BuiltInMethodParameter   m_ArrayResizeArg[];
    static BuiltInMethodParameter   m_ArrayInsertArg[];
    static BuiltInMethodParameter   m_ArrayRemoveArg[];
    static BuiltInMethodParameter   m_ArrayAddArg[];
    static BuiltInMethod            m_ArrayMethod[];

    static BuiltInMethodParameter   m_StringSubstrArg[];
    static BuiltInMethod            m_StringMethod[];
};