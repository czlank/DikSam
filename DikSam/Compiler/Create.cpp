#include "StdAfx.h"
#include "Create.h"
#include "DikSam.h"

#include "CreateC.cpp"

Create::Create(Debug& debug, Error& error, Util& util)
    : m_Debug(debug)
    , m_Error(error)
    , m_Util(util)
{

}

Create::~Create()
{

}

Declaration* Create::AllocDeclaration(TypeSpecifier *pType, char *lpstrIdentifier)
{
    Declaration *pDecl = (Declaration*)CREATE_UTIL_Malloc(sizeof(Declaration));

    pDecl->name = lpstrIdentifier;
    pDecl->type = pType;
    pDecl->variable_index = -1;

    return pDecl;
}

DeclarationList* Create::ChainDeclaration(DeclarationList *pList, Declaration *pDecl)
{
    DeclarationList *pNewItem = (DeclarationList*)CREATE_UTIL_Malloc(sizeof(DeclarationList));
    
    pNewItem->declaration = pDecl;
    pNewItem->next = nullptr;

    if (nullptr == pList)
    {
        return pNewItem;
    }

    DeclarationList *pos = pList;
    for (; pos->next != nullptr; pos = pos->next)
        ;
    pos->next = pNewItem;

    return pList;
}

void Create::FunctionDefine(DVM_BasicType enType, char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock)
{
    if (m_Util.SearchFunction(lpstrIdentifier) || m_Util.SearchDeclaration(lpstrIdentifier, nullptr))
    {
        m_Error.CompileError(m_Util.GetCompiler()->current_line_number,
            FUNCTION_MULTIPLE_DEFINE_ERR,
            STRING_MESSAGE_ARGUMENT, "name", lpstrIdentifier,
            MESSAGE_ARGUMENT_END);

        return;
    }

    FunctionDefinition  *pFD = CreateFunctionDefinition(enType, lpstrIdentifier, pParameterList, pBlock);

    if (pBlock)
    {
        pBlock->type = FUNCTION_BLOCK;
        pBlock->parent.function.function = pFD;
    }

    DKC_Compiler *pCompiler = m_Util.GetCompiler();

    if (pCompiler->function_list)
    {
        FunctionDefinition *pos = pCompiler->function_list;
        for (; pos->next; pos = pos->next)
            ;
        pos->next = pFD;
    }
    else
    {
        pCompiler->function_list = pFD;
    }
}

ParameterList* Create::CreateParameter(DVM_BasicType enType, char *lpstrIdentifier)
{
    ParameterList *p = (ParameterList*)CREATE_UTIL_Malloc(sizeof(ParameterList));

    p->name = lpstrIdentifier;
    p->type = m_Util.AllocTypeSpecifier(enType);
    p->line_number = m_Util.GetCompiler()->current_line_number;
    p->next = nullptr;

    return p;
}

FunctionDefinition* Create::CreateFunctionDefinition(DVM_BasicType enType, char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock)
{
    DKC_Compiler *pCompiler = m_Util.GetCompiler();
    FunctionDefinition *pFD = (FunctionDefinition*)CREATE_UTIL_Malloc(sizeof(FunctionDefinition));

    pFD->type = m_Util.AllocTypeSpecifier(enType);
    pFD->name = lpstrIdentifier;
    pFD->parameter = pParameterList;
    pFD->block = pBlock;
    pFD->index = pCompiler->function_count++;
    pFD->local_variable_count = 0;
    pFD->local_variable = nullptr;
    pFD->next = nullptr;

    return pFD;
}