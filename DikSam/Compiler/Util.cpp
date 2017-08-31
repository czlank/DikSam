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
    TypeSpecifier *pTypeSpecifier = (TypeSpecifier*)UTIL_STORAGE_MALLOC(sizeof(TypeSpecifier));

    pTypeSpecifier->basic_type = enType;
    pTypeSpecifier->derive = nullptr;

    return pTypeSpecifier;
}

TypeDerive* Util::AllocTypeDerive(DeriveTag enTag)
{
    TypeDerive *pTypeDerive = (TypeDerive*)UTIL_STORAGE_MALLOC(sizeof(TypeDerive));

    pTypeDerive->tag = enTag;
    pTypeDerive->next = nullptr;

    return pTypeDerive;
}

bool Util::CompareParameter(ParameterList *param1, ParameterList *param2)
{
    ParameterList *pos1;
    ParameterList *pos2;

    for (pos1 = param1, pos2 = param2; pos1 && pos2; pos1 = pos1->next, pos2 = pos2->next)
    {
        if (std::string(pos1->name) != pos2->name)
        {
            return false;
        }

        if (!CompareType(pos1->type, pos2->type))
        {
            return false;
        }
    }

    if (pos1 || pos2)
    {
        return false;
    }

    return true;
}

bool Util::CompareType(TypeSpecifier *type1, TypeSpecifier *type2)
{
    if (type1->basic_type != type2->basic_type)
    {
        return false;
    }

    TypeDerive *d1;
    TypeDerive *d2;

    for (d1 = type1->derive, d2 = type2->derive; d1 && d2; d1 = d1->next, d2 = d2->next)
    {
        if (d1->tag != d2->tag)
        {
            return false;
        }

        if (FUNCTION_DERIVE == d1->tag)
        {
            if (!CompareParameter(d1->u.function_d.parameter_list, d2->u.function_d.parameter_list))
            {
                return false;
            }
        }
    }

    if (d1 || d2)
    {
        return false;
    }

    return true;
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
    if (nullptr == vStr->string)
        return;

    m_Memory.Free(vStr->string);
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

char* Util::GetTypeName(TypeSpecifier *type)
{
    char *pTypeName = GetBasicTypeName(type->basic_type);
    char *pFinalTypeName = nullptr;

    for (TypeDerive *pos = type->derive; pos; pos = pos->next)
    {
        switch (pos->tag)
        {
        case FUNCTION_DERIVE :
            UTIL_DBG_Assert(0, ("derive_ta..%d\n", pos->tag));
            break;

        case ARRAY_DERIVE :
            {
                int iLen = std::string(pTypeName).length();

                pFinalTypeName = (char *)UTIL_MEM_MALLOC(iLen + 3);
                for (int i = 0; i < iLen; i++)
                {
                    pFinalTypeName[i] = pTypeName[i];
                }

                pFinalTypeName[iLen] = '[';
                pFinalTypeName[iLen + 1] = ']';
                pFinalTypeName[iLen + 2] = '\0';
            }
            
            break;

        default :
            UTIL_DBG_Assert(0, ("derive_ta..%d\n", pos->tag));
        }
    }

    return pFinalTypeName ? pFinalTypeName : pTypeName;
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

    default :
        return nullptr;
    }

    std::basic_string<DVM_Char> exprVal(dvmChar.str());
    int iLen = exprVal.length();
    DVM_Char *pNewStr = (DVM_Char*)UTIL_MEM_MALLOC(sizeof(DVM_Char) * (iLen + 1));
    wcscpy_s(pNewStr, iLen + 1, exprVal.c_str());

    return pNewStr;
}

int Util::StrLen(const DVM_Char *str)
{
    if (nullptr == str)
        return 0;

    return std::basic_string<DVM_Char>(str).length();
}
