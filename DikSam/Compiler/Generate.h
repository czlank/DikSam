#pragma once

#include "DikSamc.h"

class Debug;
class Memory;
class Error;

class Generate
{
    class LabelTable
    {
    public:
        int     m_iLabelAddress;

        LabelTable()
            : m_iLabelAddress(0)
        {}
    };

    class OpcodeBuf
    {
    public:
        int             m_iSize;
        int             m_iAllocSize;
        DVM_Byte        *m_pCode;
        int             m_iLabelTableSize;
        int             m_iLabelTableAllocSize;
        LabelTable      *m_pLabelTable;
        int             m_iLineNumberSize;
        DVM_LineNumber  *m_pLineNumber;

        OpcodeBuf()
            : m_iSize(0), m_iAllocSize(0)
            , m_pCode(nullptr)
            , m_iLabelTableSize(0), m_iLabelTableAllocSize(0)
            , m_pLabelTable(nullptr)
            , m_iLineNumberSize(0), m_pLineNumber(nullptr)
        {}
    };

public:
    Generate(Debug& debug, Memory& memory, Error& error);
    ~Generate();

    DVM_Executable* operator () (DKC_Compiler *pCompiler);

private:
    int AddConstantPool(DVM_Executable *pExecutable, DVM_ConstantPool *pConstantPool);
    void AddGlobalVariable(DKC_Compiler *pCompiler, DVM_Executable *pExecutable);
    int AddTypeSpecifier(TypeSpecifier *pTypeSpecifier, DVM_Executable *pExecutable);
    void AddLineNumber(OpcodeBuf *pOpcode, int iLine, int iStartPC);
    void AddFunctions(DKC_Compiler *pCompiler, DVM_Executable *pExecutable);
    void AddTopLevel(DKC_Compiler *pCompiler, DVM_Executable *pExecutable);

    void GenerateCode(OpcodeBuf *pOpcode, int iLine, DVM_Opcode code, ...);
    void GenerateBooleanExpression(DVM_Executable *pExecutable, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateIntExpression(DVM_Executable *pExecutable, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateDoubleExpression(DVM_Executable *pExecutable, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateStringExpression(DVM_Executable *pExecutable, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateIdentifierExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode);
    void GeneratePopToIdentifier(Declaration *pDeclaration, int iLine, OpcodeBuf *pOpcode);
    void GeneratePopToLValue(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateAssignExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode, bool isTopLevel);
    void GenerateBinaryExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, DVM_Opcode opCode, OpcodeBuf *pOpcode);
    void GenerateLogicalAndExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateLogicalOrExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateCastExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateArrayLiteralExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateIndexExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateIncDecExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, ExpressionKind kind, OpcodeBuf *pOpcode, bool isTopLevel);
    void GenerateFunctionCallExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateNullExpression(DVM_Executable *pExecutable, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateArrayCreationExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode);

    void GenerateExpressionStatement(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode);
    void GenerateIfStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode);
    void GenerateWhileStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode);
    void GenerateForStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode);
    void GenerateReturnStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode);
    void GenerateBreakStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode);
    void GenerateContinueStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode);
    void GenerateInitializer(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode);
    void GenerateStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode);
    void GenerateStatementList(DVM_Executable *pExecutable, Block *pBlock, StatementList *pStatementList, OpcodeBuf *pOpcode);

    DVM_Executable* AllocExecutable();
    DVM_LocalVariable* CopyParameterList(ParameterList *pParameterList, int *pParameterCount);
    DVM_LocalVariable* CopyLocalVariables(FunctionDefinition *pFunctionDefinition, int iParameterCount);
    void CopyTypeSpecifierNoAlloc(TypeSpecifier *pSrc, DVM_TypeSpecifier *pDest);
    DVM_TypeSpecifier* CopyTypeSpecifier(TypeSpecifier *pTypeSpecifier);
    int GetOpcodeTypeOffset(TypeSpecifier *pTypeSpecifier);
    int GetLabel(OpcodeBuf *pOpcode);
    void SetLabel(OpcodeBuf *pOpcode, int ilabel);
    void InitOpcodeBuf(OpcodeBuf *pOpcode);
    void FixLabels(OpcodeBuf *pOpcode);
    DVM_Byte* FixOpcodeBuf(OpcodeBuf *pOpcode);
    int CalcNeedStackSize(DVM_Byte *pCode, int iCodeSize);
    void CopyFunction(FunctionDefinition *pFunctionDefinition, DVM_Function *pFunction);

    inline bool IsString(TypeSpecifier *pType) { return (DVM_STRING_TYPE == pType->basic_type && nullptr == pType->derive); }

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
    Error   &m_Error;
};