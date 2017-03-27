#pragma once

#include "DikSamc.h"
#include "Storage.h"
#include "Memory.h"
#include "Debug.h"

class Util
{
#define UTIL_STORAGE_Malloc(size)           (m_Storage.Malloc(__FILE__, __LINE__, m_pCompiler->compile_storage, size))
#define UTIL_MEM_Malloc(size)               (m_Memory.Malloc(__FILE__, __LINE__, size))
#define UTIL_MEM_Realloc(ptr, size)         (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))
#define UTIL_DBG_Assert(expression, arg)    ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

public:
    Util(Storage& storage, Memory& memory, Debug& debug);
    ~Util();

    void SetCompiler(DKC_Compiler *pCompiler) { assert(pCompiler), m_pCompiler = pCompiler; }
    DKC_Compiler* GetCompiler() const { return m_pCompiler; }

    void* Malloc(size_t szSize);
    TypeSpecifier* AllocTypeSpecifier(DVM_BasicType enType);
    
    FunctionDefinition* SearchFunction(const char *lpcstrName);
    Declaration* SearchDeclaration(const char *lpcstrIdentifier, Block *pBlock);

    void VStrClear(VString *vStr);
    void VStrAppandString(VString *vStr, const DVM_Char *lpcstrStr);
    void VStrAppandCharacter(VString *vStr, DVM_Char ch);

    char* GetBasicTypeName(DVM_BasicType enType);
    DVM_Char* ExpressionToString(Expression *stExpr);

private:
    int StrLen(const DVM_Char *str);

private:
    DKC_Compiler    *m_pCompiler;
    Storage         &m_Storage;
    Memory          &m_Memory;
    Debug           &m_Debug;
};