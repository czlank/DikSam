#pragma once

#include "Debug.h"
#include "Error.h"
#include "Util.h"
#include "DikSamc.h"

class Create
{
#define CREATE_UTIL_Malloc(size)    (m_Util.Malloc(__FILE__, __LINE__, size))

public:
    Create(Debug& debug, Error& error, Util& util);
    ~Create();

    Declaration* AllocDeclaration(TypeSpecifier *pType, char *lpstrIdentifier);
    DeclarationList* ChainDeclaration(DeclarationList *pList, Declaration *pDecl);
    void FunctionDefine(DVM_BasicType enType, char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock);

    ParameterList* CreateParameter(DVM_BasicType enType, char *lpstrIdentifier);

private:
    FunctionDefinition* CreateFunctionDefinition(DVM_BasicType enType, char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock);

private:
    Debug&  m_Debug;
    Error&  m_Error;
    Util&   m_Util;
};