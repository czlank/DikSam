#pragma once

#include "DikSamc.h"

class Debug;
class Memory;
class Util;
class Error;
class Create;
class Interface;

class FixTree
{
public:
    FixTree(Debug& debug, Memory& memory, Util& util, Error& error, Create& create, Interface& refInterface);
    ~FixTree();

    void operator () (DKC_Compiler *pCompiler);

private:
    Expression* FixExpression(Block *pBlock, Expression *pExpression, Expression *pParentExpression);
    void FixStatement(Block *pBlock, Statement *pStatement, FunctionDefinition *pFunctionDefinition);

    Expression* FixIdentifierExpression(Block *pBlock, Expression *pExpression);
    Expression* FixCommaExpression(Block *pBlock, Expression *pExpression);
    Expression* FixAssignExpression(Block *pBlock, Expression *pExpression);
    Expression* FixMathBinaryExpression(Block *pBlock, Expression *pExpression);
    Expression* FixCompareExpression(Block *pBlock, Expression *pExpression);
    Expression* FixLogicalAndOrExpression(Block *pBlock, Expression *pExpression);
    Expression* FixMinusExpression(Block *pBlock, Expression *pExpression);
    Expression* FixLogicalNotExpression(Block *pBlock, Expression *pExpression);
    Expression* FixFunctionCallExpression(Block *pBlock, Expression *pExpression);
    Expression* FixIncDecExpression(Block *pBlock, Expression *pExpression);
    Expression* FixArrayLiteralExpression(Block *pBlock, Expression *pExpression);
    Expression* FixIndexExpression(Block *pBlock, Expression *pExpression);
    Expression* FixArrayCreationExpression(Block *pBlock, Expression *pExpression);
    Expression* FixClassMemberExpression(Expression *pExpression, Expression *pObj, char *lpstrMemberName);
    Expression* FixArrayMethodExpression(Expression *pExpression, Expression *pObj, char *lpstrMemberName);
    Expression* FixStringMethodExpression(Expression *pExpression, Expression *pObj, char *lpstrMemberName);
    Expression* FixMemberExpression(Block *pBlock, Expression *pExpression);
    
    void FixParameterList(ParameterList *pParameterList);
    void FixTypeSpecifier(TypeSpecifier *pTypeSpecifier);
    void FixIfStatement(Block *pBlock, IfStatement *pIfStatement, FunctionDefinition *pFunctionDefinition);
    void FixReturnStatement(Block *pBlock, ReturnStatement *pReturnStatement, FunctionDefinition *pFunctionDefinition);
    void FixStatementList(Block *pBlock, StatementList *pStatementList, FunctionDefinition *pFunctionDefinition);

    Expression* EvalMathExpressionInt(Expression *pExpression, int left, int right);
    Expression* EvalMathExpressionDouble(Expression *pExpression, double left, double right);
    Expression* EvalMathExpression(Block *pBlock, Expression *pExpression);
    Expression* EvalCompareExpressionBoolean(Expression *pExpression, DVM_Boolean left, DVM_Boolean right);
    Expression* EvalCompareExpressionInt(Expression *pExpression, int left, int right);
    Expression* EvalCompareExpressionDouble(Expression *pExpression, double left, double right);
    Expression* EvalCompareExpressionString(Expression *pExpression, DVM_Char *left, DVM_Char *right);
    Expression* EvalCompareExpression(Expression *pExpression);

    Expression* AllocCastExpression(CastType enType, Expression *pOperand);
    Expression* CreateUpCast(Expression *pSrc, ClassDefinition *pDestInterface, int iInterfaceIndex);
    Expression* CreateAssignCast(Expression *pSrc, TypeSpecifier *pDest);
    Expression* ChainString(Expression *pExpression);
    Expression* CastBinaryExpression(Expression *pExpression);
    Expression* CreateToStringCast(Expression *pExpression);

    void CheckArgument(Block *pBlock, int iLine, ParameterList *pParamList, ArgumentList *pArg, TypeSpecifier *pArrayBase);
    void AddLocalVariable(FunctionDefinition *pFunctionDefinition, Declaration *pDeclaration);
    void AddDeclaration(Block *pBlock, Declaration *pDeclaration, FunctionDefinition *pFunctionDefinition, int iLine);
    void AddParameterAsDeclaration(FunctionDefinition *pFunctionDefinition);
    void AddReturnFunction(FunctionDefinition *pFunctionDefinition);

    void CastMismatchError(int iLine, TypeSpecifier *pSrc, TypeSpecifier *pDest);
    int ReservFunctionIndex(DKC_Compiler *pCompiler, FunctionDefinition *pSrc);
    int AddClass(ClassDefinition *pSrc);
    bool IsSuperInterface(ClassDefinition *pChild, ClassDefinition *pParent, int *pInterfaceIndexOut);
    bool IsSuperClass(ClassDefinition *pChild, ClassDefinition *pParent, bool *pIsInterface, int *pInterfaceIndex);
    bool IsInterfaceMethod(ClassDefinition *pClassDefinition, MemberDeclaration *pMemberDeclaration, ClassDefinition **ppTargetInterface, int *pInterfaceIndexOut);
    ClassDefinition* SearchAndAddClass(int iLine, char *lpstrName, int *pClassIndex);
    TypeSpecifier* CreateFunctionDeriveType(FunctionDefinition *pFunctionDefinition);
    bool CheckTypeCompatibility(TypeSpecifier *pSuper, TypeSpecifier *pSub);
    void CheckFunctionCompatibility(int iLine, char *lpstrName, TypeSpecifier *pType1, ParameterList *pParam1, TypeSpecifier *pType2, ParameterList *pParam2);
    void CheckFunctionCompatibility(FunctionDefinition *pFunctionDefine1, FunctionDefinition *pFunctionDefine2);
    void CheckMemberAccessibility(int iLine, ClassDefinition *pTargetClass, MemberDeclaration *pMember, char *lpstrMemberName);

    inline bool IsInt(TypeSpecifier *pType) { return (DVM_INT_TYPE == pType->basic_type && nullptr == pType->derive); }
    inline bool IsDouble(TypeSpecifier *pType) { return (DVM_DOUBLE_TYPE == pType->basic_type && nullptr == pType->derive); }
    inline bool IsBoolean(TypeSpecifier *pType) { return (DVM_BOOLEAN_TYPE == pType->basic_type && nullptr == pType->derive); }
    inline bool IsString(TypeSpecifier *pType) { return (DVM_STRING_TYPE == pType->basic_type && nullptr == pType->derive); }
    inline bool IsArray(TypeSpecifier *pType) { return (pType->derive && ARRAY_DERIVE == pType->derive->tag); }
    inline bool IsClassObject(TypeSpecifier *pType) { return DVM_CLASS_TYPE == pType->basic_type && nullptr == pType->derive; }
    inline bool IsObject(TypeSpecifier *pType) { return (IsString(pType) || IsArray(pType)) || IsClassObject(pType); }

private:
    Debug       &m_Debug;
    Memory      &m_Memory;
    Util        &m_Util;
    Error       &m_Error;
    Create      &m_Create;
    Interface   &m_Interface;
};