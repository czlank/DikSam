#include "stdafx.h"
#include "Error.h"
#include "DikSam.h"
#include "Debug.h"
#include "Util.h"

#ifdef DBG_panic
#undef DBG_panic
#endif
#define DBG_panic(arg)                      (m_Debug.Panic(__FILE__, __LINE__, arg))

Error::Error(Debug& debug, Util& util)
    : m_Debug(debug)
    , m_Util(util)
{
    InitMessageFormat();
}

Error::~Error()
{

}

void Error::CompileError(int iLine, DikSamError id, ...)
{
    va_list ap;
    VString vStrMessage{ nullptr };

    SelfCheck();

    va_start(ap, id);
    FormatMessage(m_ErrorMessageFormat[id], &vStrMessage, ap);
    std::wstringstream ss;
    ss << TEXT("��") << iLine << TEXT(": ") << vStrMessage.string;
    va_end(ap);

    m_Util.VStrClear(&vStrMessage);
    throw ErrorException(ss.str().c_str());
}

void Error::DVMError(DVM_Executable *pExe, Function *pFunc, int pc, DikSamError id, ...)
{
    va_list ap;
    VString vStrMessage{ nullptr };

    SelfCheck();
    
    va_start(ap, id);
    FormatMessage(m_ErrorMessageFormat[id], &vStrMessage, ap);

    std::wstringstream ss;
    if (pc != NO_LINE_NUMBER_PC)
    {
        ss << TEXT("��") << ConvertPCToLineNumber(pExe, pFunc, pc) << TEXT(": ");
    }

    ss << vStrMessage.string;
    va_end(ap);

    m_Util.VStrClear(&vStrMessage);
    throw ErrorException(ss.str().c_str());
}

void Error::InitMessageFormat()
{
    m_ErrorMessageFormat[0]                                         = TEXT("dummy");
    m_ErrorMessageFormat[PARSE_ERR]                                 = TEXT("��($(token))���������﷨����");
    m_ErrorMessageFormat[CHARACTER_INVALID_ERR]                     = TEXT("����ȷ���ַ�($(bad_char))��");
    m_ErrorMessageFormat[FUNCTION_MULTIPLE_DEFINE_ERR]              = TEXT("�������ظ�($(name))��");
    m_ErrorMessageFormat[BAD_MULTIBYTE_CHARACTER_ERR]               = TEXT("����ȷ�Ķ��ֽ��ַ���");
    m_ErrorMessageFormat[UNEXPECTED_WIDE_STRING_IN_COMPILE_ERR]     = TEXT("Ԥ����Ŀ��ַ�����");
    m_ErrorMessageFormat[ARRAY_ELEMENT_CAN_NOT_BE_FINAL_ERR]        = TEXT("����Ԫ�ز��ܱ�ʶΪfinal��");
    m_ErrorMessageFormat[COMPLEX_ASSIGNMENT_OPERATOR_TO_FINAL_ERR]  = TEXT("���ϸ�ֵ�������������finalֵ��");
    m_ErrorMessageFormat[PARAMETER_MULTIPLE_DEFINE_ERR]             = TEXT("�����Ĳ������ظ�($(name))��");
    m_ErrorMessageFormat[VARIABLE_MULTIPLE_DEFINE_ERR]              = TEXT("������$(name)�ظ���");
    m_ErrorMessageFormat[IDENTIFIER_NOT_FOUND_ERR]                  = TEXT("�Ҳ�����������$(name)��");
    m_ErrorMessageFormat[FUNCTION_IDENTIFIER_ERR]                   = TEXT("$(name)�Ǻ���������û�к������õ�()��");
    m_ErrorMessageFormat[DERIVE_TYPE_CAST_ERR]                      = TEXT("����ǿ��ת��Ϊ�������͡�");
    m_ErrorMessageFormat[CAST_MISMATCH_ERR]                         = TEXT("���ܽ�$(src)ת��Ϊ$(dest)��");
    m_ErrorMessageFormat[MATH_TYPE_MISMATCH_ERR]                    = TEXT("����������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[COMPARE_TYPE_MISMATCH_ERR]                 = TEXT("�Ƚ�������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[LOGICAL_TYPE_MISMATCH_ERR]                 = TEXT("�߼�and/or������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[MINUS_TYPE_MISMATCH_ERR]                   = TEXT("����������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[LOGICAL_NOT_TYPE_MISMATCH_ERR]             = TEXT("�߼���������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[INC_DEC_TYPE_MISMATCH_ERR]                 = TEXT("����/�Լ�������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[FUNCTION_NOT_IDENTIFIER_ERR]               = TEXT("��������������Ĳ��������Ǻ�������");
    m_ErrorMessageFormat[FUNCTION_NOT_FOUND_ERR]                    = TEXT("�Ҳ�������$(name)��");
    m_ErrorMessageFormat[ARGUMENT_COUNT_MISMATCH_ERR]               = TEXT("�����Ĳ�����������");
    m_ErrorMessageFormat[NOT_LVALUE_ERR]                            = TEXT("��ֵ���������߲���һ����ֵ��");
    m_ErrorMessageFormat[LABEL_NOT_FOUND_ERR]                       = TEXT("��ǩ$(label)�����ڡ�");
    m_ErrorMessageFormat[ARRAY_LITERAL_EMPTY_ERR]                   = TEXT("��������������������һ��Ԫ�ء�");
    m_ErrorMessageFormat[INDEX_LEFT_OPERAND_NOT_ARRAY_ERR]          = TEXT("�±������[]����߲����������͡�");
    m_ErrorMessageFormat[INDEX_NOT_INT_ERR]                         = TEXT("������±겻��int���͡�");
    m_ErrorMessageFormat[ARRAY_SIZE_NOT_INT_ERR]                    = TEXT("����Ĵ�С����int���͡�");
    m_ErrorMessageFormat[DIVISION_BY_ZERO_ERR]                      = TEXT("����ֵ���ܱ�0����");
    m_ErrorMessageFormat[LOCAL_VARIABLE_IN_TOP_ERR]                 = TEXT("�����ڶ��������ʹ�ö���ֲ�����$(name)��");
    m_ErrorMessageFormat[INDEX_OUT_OF_BOUNDS_ERR]                   = TEXT("�����±�Խ�硣�����СΪ$(size)�����ʵ��±�Ϊ[$(index)]��");
    m_ErrorMessageFormat[NULL_POINTER_ERR]                          = TEXT("������null��");
    m_ErrorMessageFormat[PACKAGE_NAME_TOO_LONG_ERR]                 = TEXT("package���ƹ���");
    m_ErrorMessageFormat[REQUIRE_FILE_NOT_FOUND_ERR]                = TEXT("��require���ļ�������($(file))");
    m_ErrorMessageFormat[REQUIRE_FILE_ERR]                          = TEXT("requireʱ��������($(status))��");
    m_ErrorMessageFormat[REQUIRE_DUPLICATE_ERR]                     = TEXT("Դ�ļ����ظ�require�˰�($(package_name))��");
    m_ErrorMessageFormat[REQUIRE_ITSELF_ERR]                        = TEXT("����require�ļ�����");
    m_ErrorMessageFormat[RENAME_HAS_NO_PACKAGED_NAME_ERR]           = TEXT("rename������Ʊ���ָ��package��");
    m_ErrorMessageFormat[ABSTRACT_MULTIPILE_SPECIFIED_ERR]          = TEXT("�ظ�������abstract��");
    m_ErrorMessageFormat[ACCESS_MODIFIER_MULTIPLE_SPECIFIED_ERR]    = TEXT("�ظ������˷������η���");
    m_ErrorMessageFormat[OVERRIDE_MODIFIER_MULTIPLE_SPECIFIED_ERR]  = TEXT("�ظ�������override��");
    m_ErrorMessageFormat[VIRTUAL_MODIFIER_MULTIPLE_SPECIFIED_ERR]   = TEXT("�ظ�������virtual��");
    m_ErrorMessageFormat[MEMBER_EXPRESSION_TYPE_ERR]                = TEXT("�����Ͳ���ʹ�ó�Ա�������");
    m_ErrorMessageFormat[MEMBER_NOT_FOUND_ERR]                      = TEXT("������$(class_name)�в����ڳ�Ա$(member_name)��");
    m_ErrorMessageFormat[PRIVATE_MEMBER_ACCESS_ERR]                 = TEXT("���ܷ���private�ĳ�Ա$(member_name)��");
    m_ErrorMessageFormat[ABSTRACT_METHOD_HAS_BODY_ERR]              = TEXT("����ʵ��abstract������");
    m_ErrorMessageFormat[CONCRETE_METHOD_HAS_NO_BODY_ERR]           = TEXT("����ʵ�ַ�abstract������");
    m_ErrorMessageFormat[MULTIPLE_INHERITANCE_ERR]                  = TEXT("�̳��˶���ࡣ");
    m_ErrorMessageFormat[INHERIT_CONCRETE_CLASS_ERR]                = TEXT("ֻ�ܼ̳�abstract��(��$(name)����abstract��)��");
    m_ErrorMessageFormat[NEW_ABSTRACT_CLASS_ERR]                    = TEXT("���ܶ�abstract��($(name))ʹ��new��");
    m_ErrorMessageFormat[RETURN_IN_VOID_FUNCTION_ERR]               = TEXT("void���͵ĺ��������з���ֵ��");
    m_ErrorMessageFormat[CLASS_NOT_FOUND_ERR]                       = TEXT("û���ҵ���$(name)��");
    m_ErrorMessageFormat[CONSTRUCTOR_IS_NOT_METHOD_ERR]             = TEXT("��ָ��Ϊ���췽���ĳ�Ա$(member_name)����һ��������");
    m_ErrorMessageFormat[NOT_CONSTRUCTOR_ERR]                       = TEXT("����new�ķ���$(member_name)�����ǹ��췽����");
    m_ErrorMessageFormat[FIELD_CAN_NOT_CALLLED_ERR]                 = TEXT("���ܵ����ֶ�$(member_name)��");
    m_ErrorMessageFormat[METHOD_CAN_NOT_CALLED_ERR]                 = TEXT("����$(member_name)���ܳ����ں�������֮���λ�á�");
    m_ErrorMessageFormat[ASSIGN_TO_METHOD_ERR]                      = TEXT("���ܸ�ֵ������$(member_name)��");
    m_ErrorMessageFormat[NON_VIRTUAL_METHOD_OVERRIDED_ERR]          = TEXT("���ܸ��Ƿ�virtual����$(name)��");
    m_ErrorMessageFormat[NEED_OVERRIDE_ERR]                         = TEXT("���Ƿ���ʱ����ʹ��override�ؼ���($(name))��");
    m_ErrorMessageFormat[ABSTRACT_METHOD_IN_CONCRETE_CLASS_ERR]     = TEXT("��abstract���У����ڷ�abstract����$(method_name)��");
    m_ErrorMessageFormat[HASNT_SUPER_CLASS_ERR]                     = TEXT("��û�г��������ʹ����super��");
    m_ErrorMessageFormat[SUPER_NOT_IN_MEMBER_EXPRESSION_ERR]        = TEXT("�����������ⲻ��ʹ��super��");
    m_ErrorMessageFormat[FIELD_OF_SUPER_REFERENCED_ERR]             = TEXT("��������super���ֶΡ�");
    m_ErrorMessageFormat[FIELD_OVERRIDED_ERR]                       = TEXT("$(name)���ֶΣ����ܸ��ǡ�");
    m_ErrorMessageFormat[FIELD_NAME_DUPLICATE_ERR]                  = TEXT("�ظ����ֶ���$(name)��");
    m_ErrorMessageFormat[ARRAY_METHOD_NOT_FOUND_ERR]                = TEXT("����⺯����û��$(name)������");
    m_ErrorMessageFormat[STRING_METHOD_NOT_FOUND_ERR]               = TEXT("�ַ����⺯����û��$(name)������");
    m_ErrorMessageFormat[INSTANCEOF_OPERAND_NOT_REFERENCE_ERR]      = TEXT("instanceof�Ĳ������������������͡�");
    m_ErrorMessageFormat[INSTANCEOF_TYPE_NOT_REFERENCE_ERR]         = TEXT("instanceof���ұߵ����ͱ������������͡�");
    m_ErrorMessageFormat[INSTANCEOF_FOR_NOT_CLASS_ERR]              = TEXT("instanceof��Ŀ��������ࡣ");
    m_ErrorMessageFormat[INSTANCEOF_MUST_RETURN_TRUE_ERR]           = TEXT("instanceof���һֱΪ�档");
    m_ErrorMessageFormat[INSTANCEOF_MUST_RETURN_FALSE_ERR]          = TEXT("instanceof���һֱΪ�١�");
    m_ErrorMessageFormat[INSTANCEOF_INTERFACE_ERR]                  = TEXT("��Ϊ�ӿڼ�û�и��ӹ�ϵ��instanceof���һֱΪ�١�");
    m_ErrorMessageFormat[DOWN_CAST_OPERAND_IS_NOT_CLASS_ERR]        = TEXT("����ת�͵�Դ���ͱ������ࡣ");
    m_ErrorMessageFormat[DOWN_CAST_TARGET_IS_NOT_CLASS_ERR]         = TEXT("����ת�͵�Ŀ�����ͱ������ࡣ");
    m_ErrorMessageFormat[DOWN_CAST_DO_NOTHING_ERR]                  = TEXT("����Ҫ��������ת�͡�");
    m_ErrorMessageFormat[DOWN_CAST_TO_SUPER_CLASS_ERR]              = TEXT("���Խ�����ת��Ϊ���ࡣ");
    m_ErrorMessageFormat[DOWN_CAST_TO_BAD_CLASS_ERR]                = TEXT("����ת��û�м̳й�ϵ���ࡣ");
    m_ErrorMessageFormat[DOWN_CAST_INTERFACE_ERR]                   = TEXT("��Ϊ�ӿڼ�û�и��ӹ�ϵ����������ת�͡�");
    m_ErrorMessageFormat[IF_CONDITION_NOT_BOOLEAN_ERR]              = TEXT("if�����������ʽ����boolean�͡�");
    m_ErrorMessageFormat[WHILE_CONDITION_NOT_BOOLEAN_ERR]           = TEXT("while�����������ʽ����boolean�͡�");
    m_ErrorMessageFormat[FOR_CONDITION_NOT_BOOLEAN_ERR]             = TEXT("for�����������ʽ����boolean�͡�");
    m_ErrorMessageFormat[DO_WHILE_CONDITION_NOT_BOOLEAN_ERR]        = TEXT("do while�����������ʽ����boolean�͡�");
    m_ErrorMessageFormat[BAD_PARAMETER_COUNT_ERR]                   = TEXT("��������$(name)�Ĳ�����������");
    m_ErrorMessageFormat[BAD_PARAMETER_TYPE_ERR]                    = TEXT("��������$(func_name)�ĵ�$(index)������$(param_name)�����ʹ���");
    m_ErrorMessageFormat[BAD_RETURN_TYPE_ERR]                       = TEXT("��������$(name)�ķ���ֵ���ʹ���");
    m_ErrorMessageFormat[CONSTRUCTOR_CALLED_ERR]                    = TEXT("����ֱ�ӵ��ù��췽����");
    m_ErrorMessageFormat[TYPE_NAME_NOT_FOUND_ERR]                   = TEXT("�Ҳ���������$(name)��");
    m_ErrorMessageFormat[INTERFACE_INHERIT_ERR]                     = TEXT("�ӿ�֮�䲻�ܼ̳С�");
    m_ErrorMessageFormat[PACKAGE_MEMBER_ACCESS_ERR]                 = TEXT("���ܴӰ�����ʳ�Ա$(member_name)��");
    m_ErrorMessageFormat[PACKAGE_CLASS_ACCESS_ERR]                  = TEXT("���ܴӰ��������$(class_name)��");
    m_ErrorMessageFormat[THIS_OUT_OF_CLASS_ERR]                     = TEXT("����������ʹ��this��");
    m_ErrorMessageFormat[SUPER_OUT_OF_CLASS_ERR]                    = TEXT("����������ʹ��super��");
    m_ErrorMessageFormat[EOF_IN_C_COMMENT_ERR]                      = TEXT("��C��ʽ��ע������ֹ���ļ���");
    m_ErrorMessageFormat[EOF_IN_STRING_LITERAL_ERR]                 = TEXT("���ַ�������������ֹ���ļ���");
    m_ErrorMessageFormat[TOO_LONG_CHARACTER_LITERAL_ERR]            = TEXT("�ַ��������а�����2�����ϵ��ַ���");
    m_ErrorMessageFormat[DIKSAM_ERROR_COUNT_PLUS_1]                 = TEXT("dummy");
}

void Error::SelfCheck()
{
    if (std::wstring(m_ErrorMessageFormat[0]) != TEXT("dummy"))
    {
        DBG_panic(("message format error."));
    }

    if (std::wstring(m_ErrorMessageFormat[DIKSAM_ERROR_COUNT_PLUS_1]) != TEXT("dummy"))
    {
        DBG_panic(("message format error. DIKSAM_ERROR_COUNT_PLUS_1..", DIKSAM_ERROR_COUNT_PLUS_1));
    }
}

void Error::CreateMessageArgument(std::vector<MessageArgument>& vecArg, va_list ap)
{
    int iIndex = 0;
    MessageArgumentType enType;
    MessageArgument stArg;

    while ((enType = va_arg(ap, MessageArgumentType)) != MESSAGE_ARGUMENT_END)
    {
        stArg.m_enType = enType;
        stArg.m_lpcstrName = va_arg(ap, char*);

        switch (enType)
        {
        case INT_MESSAGE_ARGUMENT :
            stArg.u.m_iIntVal = va_arg(ap, int);
            break;

        case DOUBLE_MESSAGE_ARGUMENT :
            stArg.u.m_dbDoubleVal = va_arg(ap, double);
            break;

        case STRING_MESSAGE_ARGUMENT :
            stArg.u.m_lpstrStringVal = va_arg(ap, char*);
            break;

        case POINTER_MESSAGE_ARGUMENT :
            stArg.u.m_pPointerVal = va_arg(ap, void*);
            break;

        case CHARACTER_MESSAGE_ARGUMENT :
            stArg.u.m_iCharacterVal = va_arg(ap, int);
            break;

        case MESSAGE_ARGUMENT_END :
            assert(0);
            break;

        default :
            assert(0);
            break;
        }

        vecArg.push_back(stArg);
        iIndex++;
    }
}

std::vector<Error::MessageArgument>::iterator Error::SearchArgument(std::vector<Error::MessageArgument>& vecArg, const char* lpcstrName)
{
    std::vector<MessageArgument>::iterator it;

    for (it = vecArg.begin(); it != vecArg.end(); ++it)
    {
        if (std::string(it->m_lpcstrName) == lpcstrName)
            return it;
    }

    assert(0);
    return it;
}

void Error::FormatMessage(const wchar_t* lpcwstrFormat, VString *vstrMsg, va_list ap)
{
    std::vector<MessageArgument> vecArg;

    CreateMessageArgument(vecArg, ap);

    for (int i = 0; lpcwstrFormat[i] != L'\0'; i++)
    {
        if (lpcwstrFormat[i] != L'$')
        {
            m_Util.VStrAppandCharacter(vstrMsg, lpcwstrFormat[i]);
            continue;
        }

        assert(lpcwstrFormat[i + 1] == L'(');
        i += 2;

        int iStart = i, iEnd = i;
        while (lpcwstrFormat[i++] != L')') iEnd++;

        std::wstring wstrArgName;
        wstrArgName.resize(iEnd - iStart);
        
        for (i = iStart; lpcwstrFormat[i] != L')'; i++)
        {
            wstrArgName[i - iStart] = lpcwstrFormat[i];
        }

        assert(lpcwstrFormat[i] == L')');

        size_t _Dsize = 2 * wstrArgName.size() + 1;
        size_t szCount;
        char *_Dest = new char[_Dsize]();
        wcstombs_s(&szCount, _Dest, _Dsize, wstrArgName.c_str(), _Dsize - 1);
        auto it = SearchArgument(vecArg, _Dest);
        delete [] _Dest;

        assert(it != vecArg.end());

        std::wstringstream wss;
        switch (it->m_enType)
        {
        case INT_MESSAGE_ARGUMENT :
            wss << it->u.m_iIntVal;
            m_Util.VStrAppandString(vstrMsg, wss.str().c_str());
            break;

        case DOUBLE_MESSAGE_ARGUMENT :
            wss << it->u.m_dbDoubleVal;
            m_Util.VStrAppandString(vstrMsg, wss.str().c_str());
            break;

        case STRING_MESSAGE_ARGUMENT :
            wss << it->u.m_lpstrStringVal;
            m_Util.VStrAppandString(vstrMsg, wss.str().c_str());
            break;

        case POINTER_MESSAGE_ARGUMENT :
            wss << "0x" << std::hex << int(it->u.m_pPointerVal) << std::dec;
            m_Util.VStrAppandString(vstrMsg, wss.str().c_str());
            break;

        case CHARACTER_MESSAGE_ARGUMENT :
            wss << it->u.m_iCharacterVal;
            m_Util.VStrAppandString(vstrMsg, wss.str().c_str());
            break;

        case MESSAGE_ARGUMENT_END :
            assert(0);
            break;

        default:
            assert(0);
        }
    }
}


int Error::ConvertPCToLineNumber(DVM_Executable *pExe, Function *pFunc, int pc)
{
    DVM_LineNumber *pLineNumber = nullptr;
    int ret = 0;

    if (pFunc)
    {
        pLineNumber = pExe->function[pFunc->u.diksam_f.index].line_number;
    }
    else
    {
        pLineNumber = pExe->line_number;
    }

    for (int i = 0; i < pExe->line_number_size; i++)
    {
        if (pc >= pLineNumber[i].start_pc
            && pc < pLineNumber[i].start_pc + pLineNumber[i].pc_count)
        {
            ret = pLineNumber[i].line_number;
        }
    }

    return ret;
}