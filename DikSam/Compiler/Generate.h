#pragma once

#include "DikSamc.h"

class Debug;
class Memory;
class Error;

class Generate
{
#define GENERATE_MEM_Malloc(size)               (m_Memory.Malloc(__FILE__, __LINE__, size))
#define GENERATE_MEM_Realloc(ptr, size)         (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))
#define GENERATE_MEM_StrDUP(str)                (m_Memory.StrDUP(__FILE__, __LINE__, str))
#define GENERATE_DBG_Assert(expression, arg)    ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

#define OPCODE_ALLOC_SIZE   (256)

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
        DVM_LineNumber  *m_LineNumber;

        OpcodeBuf()
            : m_iSize(0), m_iAllocSize(0)
            , m_pCode(nullptr)
            , m_iLabelTableSize(0), m_iLabelTableAllocSize(0)
            , m_pLabelTable(nullptr)
            , m_iLineNumberSize(0), m_LineNumber(nullptr)
        {}
    };

public:
    Generate(Debug& debug, Memory& memory, Error& error);
    ~Generate();

    DVM_Executable* operator () (DKC_Compiler *pCompiler);

private:
    int AddConstantPool(DVM_Executable *pExecutable, DVM_ConstantPool *pConstantPool);
    void AddGlobalVariable(DKC_Compiler *pCompiler, DVM_Executable *pExecutable);
    void AddLineNumber(OpcodeBuf *pOpcodeBuf, int iLine, int iStartPC);

    void GenerateCode(OpcodeBuf *pObcode, int iLine, DVM_Opcode code, ...);

    DVM_Executable* AllocExecutable();
    DVM_LocalVariable* CopyParameterList(ParameterList *pParameterList, int *pParameterCount);
    DVM_LocalVariable* CopyLocalVariables(FunctionDefinition *pFunctionDefinition, int iParameterCount);
    DVM_TypeSpecifier* CopyTypeSpecifier(TypeSpecifier *pTypeSpecifier);

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
    Error   &m_Error;
};