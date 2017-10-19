#pragma once

#include "DikSamc.h"
#include "DVM_dev.h"

class Debug;
class Memory;
class Storage;
class Interface;

class Util
{
public:
    Util(Debug& debug, Memory& memory, Storage& storage, Interface& rInterface);
    ~Util();

    void* Malloc(const char *lpcstrFileName, int iLine, size_t szSize);
    TypeSpecifier* AllocTypeSpecifier(DVM_BasicType enType);
    TypeDerive* AllocTypeDerive(DeriveTag enTag);
    bool CompareParameter(ParameterList *param1, ParameterList *param2);
    bool CompareType(TypeSpecifier *type1, TypeSpecifier *type2);
    bool CompareString(char *str1, char* str2);
    bool ComparePackageName(PackageName *pPackageName1, PackageName *pPackageName2);
    bool ComparePackageName(char *p1, char *p2);
    
    FunctionDefinition* SearchFunction(const char *lpcstrName);
    Declaration* SearchDeclaration(const char *lpcstrIdentifier, Block *pBlock);
    Declaration* FunctionSearchDeclaration(const char *lpcstrIdentifier, Block *pBlock);
    ClassDefinition* SearchClass(char *lpstrIdentifier);

    void VStrClear(VString *vStr);
    void VStrAppandString(VString *vStr, const DVM_Char *lpcstrStr);
    void VStrAppandCharacter(VString *vStr, DVM_Char ch);

    char* GetBasicTypeName(DVM_BasicType enType);
    char* GetTypeName(TypeSpecifier *type);
    DVM_Char* ExpressionToString(Expression *stExpr);
    char* PackageNameToString(PackageName *src);
    char* CreateMethodFunctionName(char *lpstrClassName, char *lpstrMethodName);

    SearchFileStatus SearchFile(const char *lpcstrSearchPath, const char *lpcstrSearchFile, char *lpstrFoundPath, FILE **ppFp);

private:
    int StrLen(const DVM_Char *str);
    ClassDefinition* SearchRenamedClass(DKC_Compiler *pCompiler, RenameList *pRename);

private:
    Debug           &m_Debug;
    Memory          &m_Memory;
    Storage         &m_Storage;
    Interface       &m_Interface;
};