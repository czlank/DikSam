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
    ss << TEXT("行") << iLine << TEXT(": ") << vStrMessage.string;
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
        ss << TEXT("行") << ConvertPCToLineNumber(pExe, pFunc, pc) << TEXT(": ");
    }

    ss << vStrMessage.string;
    va_end(ap);

    m_Util.VStrClear(&vStrMessage);
    throw ErrorException(ss.str().c_str());
}

void Error::InitMessageFormat()
{
    m_ErrorMessageFormat[0]                                         = TEXT("dummy");
    m_ErrorMessageFormat[PARSE_ERR]                                 = TEXT("在($(token))附近发生语法错误。");
    m_ErrorMessageFormat[CHARACTER_INVALID_ERR]                     = TEXT("不正确的字符($(bad_char))。");
    m_ErrorMessageFormat[FUNCTION_MULTIPLE_DEFINE_ERR]              = TEXT("函数名重复($(name))。");
    m_ErrorMessageFormat[BAD_MULTIBYTE_CHARACTER_ERR]               = TEXT("不正确的多字节字符。");
    m_ErrorMessageFormat[UNEXPECTED_WIDE_STRING_IN_COMPILE_ERR]     = TEXT("预期外的宽字符串。");
    m_ErrorMessageFormat[ARRAY_ELEMENT_CAN_NOT_BE_FINAL_ERR]        = TEXT("数组元素不能标识为final。");
    m_ErrorMessageFormat[COMPLEX_ASSIGNMENT_OPERATOR_TO_FINAL_ERR]  = TEXT("复合赋值运算符不能用于final值。");
    m_ErrorMessageFormat[PARAMETER_MULTIPLE_DEFINE_ERR]             = TEXT("函数的参数名重复($(name))。");
    m_ErrorMessageFormat[VARIABLE_MULTIPLE_DEFINE_ERR]              = TEXT("变量名$(name)重复。");
    m_ErrorMessageFormat[IDENTIFIER_NOT_FOUND_ERR]                  = TEXT("找不到变量或函数$(name)。");
    m_ErrorMessageFormat[FUNCTION_IDENTIFIER_ERR]                   = TEXT("$(name)是函数名，但没有函数调用的()。");
    m_ErrorMessageFormat[DERIVE_TYPE_CAST_ERR]                      = TEXT("不能强制转型为派生类型。");
    m_ErrorMessageFormat[CAST_MISMATCH_ERR]                         = TEXT("不能将$(src)转型为$(dest)。");
    m_ErrorMessageFormat[MATH_TYPE_MISMATCH_ERR]                    = TEXT("算数运算符的操作数类型不正确。");
    m_ErrorMessageFormat[COMPARE_TYPE_MISMATCH_ERR]                 = TEXT("比较运算符的操作数类型不正确。");
    m_ErrorMessageFormat[LOGICAL_TYPE_MISMATCH_ERR]                 = TEXT("逻辑and/or运算符的操作数类型不正确。");
    m_ErrorMessageFormat[MINUS_TYPE_MISMATCH_ERR]                   = TEXT("减法运算符的操作数类型不正确。");
    m_ErrorMessageFormat[LOGICAL_NOT_TYPE_MISMATCH_ERR]             = TEXT("逻辑非运算符的操作数类型不正确。");
    m_ErrorMessageFormat[INC_DEC_TYPE_MISMATCH_ERR]                 = TEXT("自增/自减运算符的操作数类型不正确。");
    m_ErrorMessageFormat[FUNCTION_NOT_IDENTIFIER_ERR]               = TEXT("函数调用运算符的操作数不是函数名。");
    m_ErrorMessageFormat[FUNCTION_NOT_FOUND_ERR]                    = TEXT("找不到函数$(name)。");
    m_ErrorMessageFormat[ARGUMENT_COUNT_MISMATCH_ERR]               = TEXT("函数的参数数量错误。");
    m_ErrorMessageFormat[NOT_LVALUE_ERR]                            = TEXT("赋值运算符的左边不是一个左值。");
    m_ErrorMessageFormat[LABEL_NOT_FOUND_ERR]                       = TEXT("标签$(label)不存在。");
    m_ErrorMessageFormat[ARRAY_LITERAL_EMPTY_ERR]                   = TEXT("数组字面量必须至少有一个元素。");
    m_ErrorMessageFormat[INDEX_LEFT_OPERAND_NOT_ARRAY_ERR]          = TEXT("下标运算符[]的左边不是数组类型。");
    m_ErrorMessageFormat[INDEX_NOT_INT_ERR]                         = TEXT("数组的下标不是int类型。");
    m_ErrorMessageFormat[ARRAY_SIZE_NOT_INT_ERR]                    = TEXT("数组的大小不是int类型。");
    m_ErrorMessageFormat[DIVISION_BY_ZERO_ERR]                      = TEXT("整数值不能被0除。");
    m_ErrorMessageFormat[LOCAL_VARIABLE_IN_TOP_ERR]                 = TEXT("不能在顶层代码中使用定义局部变量$(name)。");
    m_ErrorMessageFormat[INDEX_OUT_OF_BOUNDS_ERR]                   = TEXT("数组下标越界。数组大小为$(size)，访问的下标为[$(index)]。");
    m_ErrorMessageFormat[NULL_POINTER_ERR]                          = TEXT("引用了null。");
    m_ErrorMessageFormat[PACKAGE_NAME_TOO_LONG_ERR]                 = TEXT("package名称过长");
    m_ErrorMessageFormat[REQUIRE_FILE_NOT_FOUND_ERR]                = TEXT("被require的文件不存在($(file))");
    m_ErrorMessageFormat[REQUIRE_FILE_ERR]                          = TEXT("require时发生错误($(status))。");
    m_ErrorMessageFormat[REQUIRE_DUPLICATE_ERR]                     = TEXT("源文件中重复require了包($(package_name))。");
    m_ErrorMessageFormat[REQUIRE_ITSELF_ERR]                        = TEXT("不能require文件本身。");
    m_ErrorMessageFormat[RENAME_HAS_NO_PACKAGED_NAME_ERR]           = TEXT("rename后的名称必须指定package。");
    m_ErrorMessageFormat[ABSTRACT_MULTIPILE_SPECIFIED_ERR]          = TEXT("重复声明了abstract。");
    m_ErrorMessageFormat[ACCESS_MODIFIER_MULTIPLE_SPECIFIED_ERR]    = TEXT("重复声明了访问修饰符。");
    m_ErrorMessageFormat[OVERRIDE_MODIFIER_MULTIPLE_SPECIFIED_ERR]  = TEXT("重复声明了override。");
    m_ErrorMessageFormat[VIRTUAL_MODIFIER_MULTIPLE_SPECIFIED_ERR]   = TEXT("重复声明了virtual。");
    m_ErrorMessageFormat[MEMBER_EXPRESSION_TYPE_ERR]                = TEXT("该类型不能使用成员运算符。");
    m_ErrorMessageFormat[MEMBER_NOT_FOUND_ERR]                      = TEXT("在类型$(class_name)中不存在成员$(member_name)。");
    m_ErrorMessageFormat[PRIVATE_MEMBER_ACCESS_ERR]                 = TEXT("不能访问private的成员$(member_name)。");
    m_ErrorMessageFormat[ABSTRACT_METHOD_HAS_BODY_ERR]              = TEXT("不能实现abstract方法。");
    m_ErrorMessageFormat[CONCRETE_METHOD_HAS_NO_BODY_ERR]           = TEXT("必须实现非abstract方法。");
    m_ErrorMessageFormat[MULTIPLE_INHERITANCE_ERR]                  = TEXT("继承了多个类。");
    m_ErrorMessageFormat[INHERIT_CONCRETE_CLASS_ERR]                = TEXT("只能继承abstract类(类$(name)不是abstract类)。");
    m_ErrorMessageFormat[NEW_ABSTRACT_CLASS_ERR]                    = TEXT("不能对abstract类($(name))使用new。");
    m_ErrorMessageFormat[RETURN_IN_VOID_FUNCTION_ERR]               = TEXT("void类型的函数不能有返回值。");
    m_ErrorMessageFormat[CLASS_NOT_FOUND_ERR]                       = TEXT("没有找到类$(name)。");
    m_ErrorMessageFormat[CONSTRUCTOR_IS_NOT_METHOD_ERR]             = TEXT("被指定为构造方法的成员$(member_name)不是一个方法。");
    m_ErrorMessageFormat[NOT_CONSTRUCTOR_ERR]                       = TEXT("用来new的方法$(member_name)并不是构造方法。");
    m_ErrorMessageFormat[FIELD_CAN_NOT_CALLLED_ERR]                 = TEXT("不能调用字段$(member_name)。");
    m_ErrorMessageFormat[METHOD_CAN_NOT_CALLED_ERR]                 = TEXT("方法$(member_name)不能出现在函数调用之外的位置。");
    m_ErrorMessageFormat[ASSIGN_TO_METHOD_ERR]                      = TEXT("不能赋值给方法$(member_name)。");
    m_ErrorMessageFormat[NON_VIRTUAL_METHOD_OVERRIDED_ERR]          = TEXT("不能覆盖非virtual方法$(name)。");
    m_ErrorMessageFormat[NEED_OVERRIDE_ERR]                         = TEXT("覆盖方法时必须使用override关键字($(name))。");
    m_ErrorMessageFormat[ABSTRACT_METHOD_IN_CONCRETE_CLASS_ERR]     = TEXT("在abstract类中，存在非abstract方法$(method_name)。");
    m_ErrorMessageFormat[HASNT_SUPER_CLASS_ERR]                     = TEXT("在没有超类的类中使用了super。");
    m_ErrorMessageFormat[SUPER_NOT_IN_MEMBER_EXPRESSION_ERR]        = TEXT("方法调用以外不能使用super。");
    m_ErrorMessageFormat[FIELD_OF_SUPER_REFERENCED_ERR]             = TEXT("不能引用super的字段。");
    m_ErrorMessageFormat[FIELD_OVERRIDED_ERR]                       = TEXT("$(name)是字段，不能覆盖。");
    m_ErrorMessageFormat[FIELD_NAME_DUPLICATE_ERR]                  = TEXT("重复的字段名$(name)。");
    m_ErrorMessageFormat[ARRAY_METHOD_NOT_FOUND_ERR]                = TEXT("数组库函数中没有$(name)方法。");
    m_ErrorMessageFormat[STRING_METHOD_NOT_FOUND_ERR]               = TEXT("字符串库函数中没有$(name)方法。");
    m_ErrorMessageFormat[INSTANCEOF_OPERAND_NOT_REFERENCE_ERR]      = TEXT("instanceof的操作数必须是引用类型。");
    m_ErrorMessageFormat[INSTANCEOF_TYPE_NOT_REFERENCE_ERR]         = TEXT("instanceof的右边的类型必须是引用类型。");
    m_ErrorMessageFormat[INSTANCEOF_FOR_NOT_CLASS_ERR]              = TEXT("instanceof的目标必须是类。");
    m_ErrorMessageFormat[INSTANCEOF_MUST_RETURN_TRUE_ERR]           = TEXT("instanceof语句一直为真。");
    m_ErrorMessageFormat[INSTANCEOF_MUST_RETURN_FALSE_ERR]          = TEXT("instanceof语句一直为假。");
    m_ErrorMessageFormat[INSTANCEOF_INTERFACE_ERR]                  = TEXT("因为接口间没有父子关系，instanceof语句一直为假。");
    m_ErrorMessageFormat[DOWN_CAST_OPERAND_IS_NOT_CLASS_ERR]        = TEXT("向下转型的源类型必须是类。");
    m_ErrorMessageFormat[DOWN_CAST_TARGET_IS_NOT_CLASS_ERR]         = TEXT("向下转型的目标类型必须是类。");
    m_ErrorMessageFormat[DOWN_CAST_DO_NOTHING_ERR]                  = TEXT("不需要进行向下转型。");
    m_ErrorMessageFormat[DOWN_CAST_TO_SUPER_CLASS_ERR]              = TEXT("尝试将父类转换为子类。");
    m_ErrorMessageFormat[DOWN_CAST_TO_BAD_CLASS_ERR]                = TEXT("尝试转换没有继承关系的类。");
    m_ErrorMessageFormat[DOWN_CAST_INTERFACE_ERR]                   = TEXT("因为接口间没有父子关系，不能向下转型。");
    m_ErrorMessageFormat[IF_CONDITION_NOT_BOOLEAN_ERR]              = TEXT("if语句的条件表达式不是boolean型。");
    m_ErrorMessageFormat[WHILE_CONDITION_NOT_BOOLEAN_ERR]           = TEXT("while语句的条件表达式不是boolean型。");
    m_ErrorMessageFormat[FOR_CONDITION_NOT_BOOLEAN_ERR]             = TEXT("for语句的条件表达式不是boolean型。");
    m_ErrorMessageFormat[DO_WHILE_CONDITION_NOT_BOOLEAN_ERR]        = TEXT("do while语句的条件表达式不是boolean型。");
    m_ErrorMessageFormat[BAD_PARAMETER_COUNT_ERR]                   = TEXT("方法或函数$(name)的参数数量错误。");
    m_ErrorMessageFormat[BAD_PARAMETER_TYPE_ERR]                    = TEXT("方法或函数$(func_name)的第$(index)个参数$(param_name)的类型错误。");
    m_ErrorMessageFormat[BAD_RETURN_TYPE_ERR]                       = TEXT("方法或函数$(name)的返回值类型错误。");
    m_ErrorMessageFormat[CONSTRUCTOR_CALLED_ERR]                    = TEXT("不能直接调用构造方法。");
    m_ErrorMessageFormat[TYPE_NAME_NOT_FOUND_ERR]                   = TEXT("找不到类型名$(name)。");
    m_ErrorMessageFormat[INTERFACE_INHERIT_ERR]                     = TEXT("接口之间不能继承。");
    m_ErrorMessageFormat[PACKAGE_MEMBER_ACCESS_ERR]                 = TEXT("不能从包外访问成员$(member_name)。");
    m_ErrorMessageFormat[PACKAGE_CLASS_ACCESS_ERR]                  = TEXT("不能从包外访问类$(class_name)。");
    m_ErrorMessageFormat[THIS_OUT_OF_CLASS_ERR]                     = TEXT("不能在类外使用this。");
    m_ErrorMessageFormat[SUPER_OUT_OF_CLASS_ERR]                    = TEXT("不能在类外使用super。");
    m_ErrorMessageFormat[EOF_IN_C_COMMENT_ERR]                      = TEXT("在C样式的注释中终止了文件。");
    m_ErrorMessageFormat[EOF_IN_STRING_LITERAL_ERR]                 = TEXT("在字符串字面量中终止了文件。");
    m_ErrorMessageFormat[TOO_LONG_CHARACTER_LITERAL_ERR]            = TEXT("字符字面量中包含了2个以上的字符。");
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