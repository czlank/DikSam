#include "stdafx.h"
#include "Util.h"
#include "DikSam.h"
#include "Debug.h"
#include "Memory.h"
#include "Storage.h"
#include "Interface.h"

Util::Util(Debug& debug, Memory& memory, Storage& storage, Interface& rInterface)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Storage(storage)
    , m_Interface(rInterface)
{

}

Util::~Util()
{

}

void* Util::Malloc(const char *lpcstrFileName, int iLine, size_t szSize)
{
    void *p = m_Storage.Malloc(lpcstrFileName, iLine, m_Interface.GetCompiler()->compile_storage, szSize);
    return p;
}

TypeSpecifier* Util::AllocTypeSpecifier(DVM_BasicType enType)
{
    TypeSpecifier *ts = (TypeSpecifier*)Malloc(__FILE__, __LINE__, sizeof(TypeSpecifier));

    ts->basic_type = enType;
    ts->derive = nullptr;

    return ts;
}

FunctionDefinition* Util::SearchFunction(const char *lpcstrName)
{
    FunctionDefinition  *pos = nullptr;

    for (pos = m_Interface.GetCompiler()->function_list; pos; pos = pos->next)
    {
        if (std::string(pos->name) == lpcstrName)
            break;
    }

    return pos;
}

Declaration* Util::SearchDeclaration(const char *lpcstrIdentifier, Block *pBlock)
{
    for (Block *bPos = pBlock; bPos; bPos = bPos->outer_block)
    {
        for (DeclarationList *dPos = bPos->declaration_list; dPos; dPos = dPos->next)
        {
            if (std::string(lpcstrIdentifier) == dPos->declaration->name)
            {
                return dPos->declaration;
            }
        }
    }

    for (DeclarationList *dPos = m_Interface.GetCompiler()->declaration_list; dPos; dPos = dPos->next)
    {
        if (std::string(lpcstrIdentifier) == dPos->declaration->name)
        {
            return dPos->declaration;
        }
    }

    return nullptr;
}

void Util::VStrClear(VString *vStr)
{
    vStr->string = nullptr;
}

void Util::VStrAppandString(VString *vStr, const DVM_Char *lpcstrStr)
{
    int iOldLen = StrLen(vStr->string);
    int iNewLen = iOldLen + StrLen(lpcstrStr) + 1;

    vStr->string = (DVM_Char*)UTIL_MEM_Realloc(vStr->string, sizeof(DVM_Char) * iNewLen);
    wcscpy_s(&vStr->string[iOldLen], iNewLen - iOldLen, lpcstrStr);
}

void Util::VStrAppandCharacter(VString *vStr, DVM_Char ch)
{
    int iCurrentLen = StrLen(vStr->string);

    vStr->string = (DVM_Char*)UTIL_MEM_Realloc(vStr->string, sizeof(DVM_Char) * (iCurrentLen + 2));
    vStr->string[iCurrentLen] = ch;
    vStr->string[iCurrentLen + 1] = TEXT('\0');
}

char* Util::GetBasicTypeName(DVM_BasicType enType)
{
    switch (enType)
    {
    case DVM_BOOLEAN_TYPE :
        return "boolean";

    case DVM_INT_TYPE :
        return "int";

    case DVM_DOUBLE_TYPE :
        return "double";

    case DVM_STRING_TYPE :
        return "string";

    default :
        UTIL_DBG_Assert(0, ("bad case. type..", enType));
    }

    return nullptr;
}

DVM_Char* Util::ExpressionToString(Expression *stExpr)
{
    std::basic_stringstream<DVM_Char> dvmChar;

    switch (stExpr->kind)
    {
    case BOOLEAN_EXPRESSION :
        dvmChar << (DVM_TRUE == stExpr->u.boolean_value) ? TEXT("true") : TEXT("false");
        break;

    case INT_EXPRESSION :
        dvmChar << stExpr->u.int_value;
        break;

    case DOUBLE_EXPRESSION :
        dvmChar << stExpr->u.double_value;
        break;

    case STRING_EXPRESSION :
        return stExpr->u.string_value;
    }

    std::basic_string<DVM_Char> exprVal(dvmChar.str());
    int iLen = exprVal.length();
    DVM_Char *pNewStr = (DVM_Char*)UTIL_MEM_Malloc(sizeof(DVM_Char)* (iLen + 1));
    wcscpy_s(pNewStr, iLen + 1, exprVal.c_str());

    return pNewStr;
}

int Util::StrLen(const DVM_Char *str)
{
    if (nullptr == str)
        return 0;

    return std::basic_string<DVM_Char>(str).length();
}
