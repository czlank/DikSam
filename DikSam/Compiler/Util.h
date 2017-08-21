#pragma once

#include "DikSamc.h"

class Debug;
class Memory;
class Storage;
class Interface;

class Util
{
#define UTIL_MEM_MALLOC(size)               (m_Memory.Malloc(__FILE__, __LINE__, size))
#define UTIL_MEM_Realloc(ptr, size)         (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))
#define UTIL_STORAGE_MALLOC(size)           (Malloc(__FILE__, __LINE__, size))
#define UTIL_DBG_Assert(expression, arg)    ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

public:
    Util(Debug& debug, Memory& memory, Storage& storage, Interface& rInterface);
    ~Util();

    void* Malloc(const char *lpcstrFileName, int iLine, size_t szSize);
    TypeSpecifier* AllocTypeSpecifier(DVM_BasicType enType);
    TypeDerive* AllocTypeDerive(DeriveTag enTag);
    bool CompareParameter(ParameterList *param1, ParameterList *param2);
    bool CompareType(TypeSpecifier *type1, TypeSpecifier *type2);
    
    FunctionDefinition* SearchFunction(const char *lpcstrName);
    Declaration* SearchDeclaration(const char *lpcstrIdentifier, Block *pBlock);

    void VStrClear(VString *vStr);
    void VStrAppandString(VString *vStr, const DVM_Char *lpcstrStr);
    void VStrAppandCharacter(VString *vStr, DVM_Char ch);

    char* GetBasicTypeName(DVM_BasicType enType);
    char* GetTypeName(TypeSpecifier *type);
    DVM_Char* ExpressionToString(Expression *stExpr);

private:
    int StrLen(const DVM_Char *str);

private:
    Debug           &m_Debug;
    Memory          &m_Memory;
    Storage         &m_Storage;
    Interface       &m_Interface;
};