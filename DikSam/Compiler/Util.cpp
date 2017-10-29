#include "stdafx.h"
#include "Util.h"
#include "DikSam.h"
#include "Debug.h"
#include "Memory.h"
#include "Storage.h"
#include "Interface.h"

#ifdef MEM_malloc
#undef MEM_malloc
#endif
#define MEM_malloc(size)                    (m_Memory.Malloc(__FILE__, __LINE__, size))

#ifdef MEM_realloc
#undef MEM_realloc
#endif
#define MEM_realloc(ptr, size)              (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))

#ifdef dkc_malloc
#undef dkc_malloc
#endif
#define dkc_malloc(size)                    (Malloc(__FILE__, __LINE__, size))

#ifdef DBG_assert
#undef DBG_assert
#endif
#define DBG_assert(expression, arg) ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

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
    void *p = m_Storage.Malloc(lpcstrFileName, iLine, m_Interface.GetCurrentCompiler()->compile_storage, szSize);
    return p;
}

TypeSpecifier* Util::AllocTypeSpecifier(DVM_BasicType enType)
{
    TypeSpecifier *pTypeSpecifier = (TypeSpecifier*)dkc_malloc(sizeof(TypeSpecifier));

    pTypeSpecifier->basic_type = enType;
    pTypeSpecifier->derive = nullptr;

    return pTypeSpecifier;
}

TypeSpecifier* Util::AllocTypeSpecifier(TypeSpecifier *pSrc)
{
    TypeSpecifier *pTypeSpecifier = (TypeSpecifier*)dkc_malloc(sizeof(TypeSpecifier));

    *pTypeSpecifier = *pSrc;

    return pTypeSpecifier;
}

TypeDerive* Util::AllocTypeDerive(DeriveTag enTag)
{
    TypeDerive *pTypeDerive = (TypeDerive*)dkc_malloc(sizeof(TypeDerive));

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

bool Util::CompareString(char *str1, char* str2)
{
    if (nullptr == str1 && nullptr == str2)
        return true;

    if (nullptr == str1 || nullptr == str2)
        return false;

    return std::string(str1) == str2;
}

bool Util::ComparePackageName(PackageName *pPackageName1, PackageName *pPackageName2)
{
    PackageName *pos1 = pPackageName1;
    PackageName *pos2 = pPackageName2;

    for (; pos1 && pos2; pos1 = pos1->next, pos2 = pos2->next)
    {
        if (std::string(pos1->name) != pos2->name)
            return false;
    }

    return !(pos1 || pos2);
}

bool Util::ComparePackageName(char *p1, char *p2)
{
    return CompareString(p1, p2);
}

FunctionDefinition* Util::SearchFunction(const char *lpcstrName)
{
    FunctionDefinition  *pos = nullptr;

    for (pos = m_Interface.GetCurrentCompiler()->function_list; pos; pos = pos->next)
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

    for (DeclarationList *dPos = m_Interface.GetCurrentCompiler()->declaration_list; dPos; dPos = dPos->next)
    {
        if (std::string(lpcstrIdentifier) == dPos->declaration->name)
        {
            return dPos->declaration;
        }
    }

    return nullptr;
}

Declaration* Util::FunctionSearchDeclaration(const char *lpcstrIdentifier, Block *pBlock)
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

    return nullptr;
}

ClassDefinition* Util::SearchClass(char *lpstrIdentifier)
{
    DKC_Compiler *pCompiler = m_Interface.GetCurrentCompiler();

    for (ClassDefinition *pos = pCompiler->class_definition_list; pos; pos = pos->next)
    {
        if (lpstrIdentifier == std::string(pos->name))
        {
            return pos;
        }
    }

    for (RenameList *pos = pCompiler->rename_list; pos; pos = pos->next)
    {
        if (lpstrIdentifier == std::string(pos->renamed_name))
        {
            if (ClassDefinition *pClassDefinition = SearchRenamedClass(pCompiler, pos))
            {
                return pClassDefinition;
            }
        }
    }

    for (CompilerList *pos = pCompiler->required_list; pos; pos = pos->next)
    {
        for (ClassDefinition *pClassDefinition = pos->compiler->class_definition_list; pClassDefinition; pClassDefinition = pClassDefinition->next)
        {
            if (lpstrIdentifier == std::string(pClassDefinition->name))
            {
                return pClassDefinition;
            }
        }
    }

    return nullptr;
}

MemberDeclaration* Util::SearchMember(ClassDefinition *pClassDefinition, char *lpstrMemberName)
{
    MemberDeclaration *pMemberDeclaration = pClassDefinition->member;

    for (; pMemberDeclaration; pMemberDeclaration = pMemberDeclaration->next)
    {
        if (METHOD_MEMBER == pMemberDeclaration->kind)
        {
            if (std::string(pMemberDeclaration->u.method.function_definition->name) == lpstrMemberName)
            {
                break;
            }
        }
        else
        {
            DBG_assert(FIELD_MEMBER == pMemberDeclaration->kind, ("member..", pMemberDeclaration->kind));

            if (std::string(pMemberDeclaration->u.field.name) == lpstrMemberName)
            {
                break;
            }
        }
    }

    if (pMemberDeclaration)
    {
        return pMemberDeclaration;
    }

    if (pClassDefinition->super_class)
    {
        pMemberDeclaration = SearchMember(pClassDefinition->super_class, lpstrMemberName);
    }

    if (pMemberDeclaration)
    {
        return pMemberDeclaration;
    }

    for (ExtendsList *pExtends = pClassDefinition->interface_list; pExtends; pExtends = pExtends->next)
    {
        pMemberDeclaration = SearchMember(pExtends->class_definition, lpstrMemberName);

        if (pMemberDeclaration)
        {
            return pMemberDeclaration;
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

    vStr->string = (DVM_Char*)MEM_realloc(vStr->string, sizeof(DVM_Char) * iNewLen);
    wcscpy_s(&vStr->string[iOldLen], iNewLen - iOldLen, lpcstrStr);
}

void Util::VStrAppandCharacter(VString *vStr, DVM_Char ch)
{
    int iCurrentLen = StrLen(vStr->string);

    vStr->string = (DVM_Char*)MEM_realloc(vStr->string, sizeof(DVM_Char) * (iCurrentLen + 2));
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
        DBG_assert(0, ("bad case. type..", enType));
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
            DBG_assert(0, ("derive_ta..%d\n", pos->tag));
            break;

        case ARRAY_DERIVE :
            {
                int iLen = std::string(pTypeName).length();

                pFinalTypeName = (char *)MEM_malloc(iLen + 3);
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
            DBG_assert(0, ("derive_ta..%d\n", pos->tag));
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
    DVM_Char *pNewStr = (DVM_Char*)MEM_malloc(sizeof(DVM_Char) * (iLen + 1));
    wcscpy_s(pNewStr, iLen + 1, exprVal.c_str());

    return pNewStr;
}

char* Util::PackageNameToString(PackageName *src)
{
    if (nullptr == src)
        return nullptr;

    std::string packageName;
    for (PackageName *pos = src; pos; pos = pos->next)
    {
        if (pos->name)
        {
            packageName += pos->name;
        }
        
        if (pos->next)
        {
            packageName += ".";
        }   
    }

    char *dest = (char*)MEM_malloc(packageName.length() + 1);
    
    for (size_t i = 0; i < packageName.length(); i++)
    {
        dest[i] = packageName[i];
    }

    dest[packageName.length()] = '\0';

    return dest;
}

char* Util::CreateMethodFunctionName(char *lpstrClassName, char *lpstrMethodName)
{
    int iClassNameLen = lpstrClassName ? std::string(lpstrClassName).length() : 0;
    int iMethodNameLen = lpstrMethodName ? std::string(lpstrMethodName).length() : 0;

    char *ret = (char*)MEM_malloc(iClassNameLen + iMethodNameLen + 2);

    int i = 0;
    if (iClassNameLen)
    {
        for (; i < iClassNameLen; i++)
        {
            ret[i] = lpstrClassName[i];
        }
    }

    ret[i++] = '#';

    if (iMethodNameLen)
    {
        for (int j = 0; j < iMethodNameLen; j++, i++)
        {
            ret[i] = lpstrMethodName[j];
        }
    }

    ret[i] = '\0';

    return ret;
}

SearchFileStatus Util::SearchFile(const char *lpcstrSearchPath, const char *lpcstrSearchFile, char *lpstrFoundPath, FILE **ppFp)
{
    size_t iSearchFileLen = std::string(lpcstrSearchFile).length();
    char lpstrDirPath[FILENAME_MAX];

    for (int spIdx = 0, dpIdx = 0; ; spIdx++)
    {
        if (FILE_PATH_SEPARATOR == lpcstrSearchPath[spIdx] || '\0' == lpcstrSearchPath[spIdx])
        {
            if (dpIdx + 1 + iSearchFileLen >= FILENAME_MAX - 1)
            {
                return SEARCH_FILE_PATH_TOO_LONG;
            }

            if (dpIdx > 0 && lpstrDirPath[dpIdx - 1] != FILE_SEPARATOR)
            {
                lpstrDirPath[dpIdx++] = FILE_SEPARATOR;
            }

            for (size_t i = 0; i < iSearchFileLen; i++)
            {
                lpstrDirPath[dpIdx + i] = lpcstrSearchFile[i];
            }

            FILE *fpTemp = nullptr;
            fopen_s(&fpTemp, lpstrDirPath, "r");
            
            if (fpTemp)
            {
                *ppFp = fpTemp;

                for (size_t i = 0; i < std::string(lpstrDirPath).length(); i++)
                {
                    lpstrFoundPath[i] = lpstrDirPath[i];
                }

                return SEARCH_FILE_SUCCESS;
            }

            dpIdx = 0;

            if ('\0' == lpcstrSearchPath[spIdx])
            {
                return SEARCH_FILE_NOT_FOUND;
            }
        }
        else
        {
            char *lpstrHome = nullptr;
            size_t len = 0;
            errno_t err = _dupenv_s(&lpstrHome, &len, "DKM_REQUIRE_SEARCH_PATH");

            if (err)
            {
                if (lpstrHome)
                {
                    free(lpstrHome);
                }

                DBG_assert(0, ("can't find environment variable DKM_REQUIRE_SEARCH_PATH, err = ", err));
            }

            if (0 == dpIdx && '~' == lpcstrSearchPath[spIdx] && lpstrHome)
            {
                for (size_t i = 0; i < std::string(lpstrHome).length(); i++)
                {
                    lpstrDirPath[dpIdx + i] = lpstrHome[i];
                }

                dpIdx += std::string(lpstrHome).length();
            }
            else
            {
                if (dpIdx >= FILENAME_MAX - 1)
                {
                    return SEARCH_FILE_PATH_TOO_LONG;
                }

                lpstrDirPath[dpIdx++] = lpcstrSearchPath[spIdx];
            }
        }
    }

    DBG_assert(0, ("bad flow."));
}

int Util::StrLen(const DVM_Char *str)
{
    if (nullptr == str)
        return 0;

    return std::basic_string<DVM_Char>(str).length();
}

ClassDefinition* Util::SearchRenamedClass(DKC_Compiler *pCompiler, RenameList *pRename)
{
    for (CompilerList *pos = pCompiler->required_list; pos; pos = pos->next)
    {
        if (!ComparePackageName(pRename->package_name, pos->compiler->package_name))
        {
            continue;
        }

        for (ClassDefinition *pClassDefinition = pos->compiler->class_definition_list; pClassDefinition; pClassDefinition = pClassDefinition->next)
        {
            if (std::string(pClassDefinition->name) == pRename->original_name)
            {
                return pClassDefinition;
            }
        }
    }

    return nullptr;
}