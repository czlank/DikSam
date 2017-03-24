#include "stdafx.h"
#include "Error.h"

Error::Error(Util& util, Debug& debug)
    : m_Util(util)
    , m_Debug(debug)
{
    InitMessageFormat();
}

Error::~Error()
{

}

void Error::CompileError(int iLine, DikSamError id, ...)
{
    va_list ap;
    VString vStrMessage;

    SelfCheck();
    va_start(ap, id);
    m_Util.VStrClear(&vStrMessage);
    FormatMessage(iLine, m_ErrorMessageFormat[id], &vStrMessage, ap);
    std::wstringstream ss;
    ss << iLine << TEXT(" ") << vStrMessage.string;
    va_end(ap);

    throw ErrorException(ss.str().c_str());
}

void Error::InitMessageFormat()
{
    m_ErrorMessageFormat[0] = TEXT("dummy");
    m_ErrorMessageFormat[1] = TEXT("在($(token))附近发生语法错误。");
    m_ErrorMessageFormat[2] = TEXT("不正确的字符($(bad_char))。");
    m_ErrorMessageFormat[3] = TEXT("函数名重复($(name))。");
    m_ErrorMessageFormat[4] = TEXT("不正确的多字节字符。");
    m_ErrorMessageFormat[5] = TEXT("预期外的宽字符串。");
    m_ErrorMessageFormat[6] = TEXT("函数的参数名重复($(name))。");
    m_ErrorMessageFormat[7] = TEXT("变量名$(name)重复。");
    m_ErrorMessageFormat[8] = TEXT("找不到变量或函数$(name)。");
    m_ErrorMessageFormat[9] = TEXT("不能强制转型为派生类型。");
    m_ErrorMessageFormat[10] = TEXT("不能将$(src)转型为$(dest)。");
    m_ErrorMessageFormat[11] = TEXT("算数运算符的操作数类型不正确。");
    m_ErrorMessageFormat[12] = TEXT("比较运算符的操作数类型不正确。");
    m_ErrorMessageFormat[13] = TEXT("逻辑and/or运算符的操作数类型不正确。");
    m_ErrorMessageFormat[14] = TEXT("减法运算符的操作数类型不正确。");
    m_ErrorMessageFormat[15] = TEXT("逻辑非运算符的操作数类型不正确。");
    m_ErrorMessageFormat[16] = TEXT("自增/自减运算符的操作数类型不正确。");
    m_ErrorMessageFormat[17] = TEXT("函数调用运算符的操作数不是函数名。");
    m_ErrorMessageFormat[18] = TEXT("找不到函数$(name)。");
    m_ErrorMessageFormat[19] = TEXT("函数的参数数量错误。");
    m_ErrorMessageFormat[20] = TEXT("赋值运算符的左边不是一个左值。");
    m_ErrorMessageFormat[21] = TEXT("标签$(label)不存在。");
    m_ErrorMessageFormat[22] = TEXT("dummy");
}

void Error::SelfCheck()
{
    if (std::wstring(m_ErrorMessageFormat[0]) != TEXT("dummy"))
    {
        ERROR_DBG_Panic(("message format error."));
    }

    if (std::wstring(m_ErrorMessageFormat[DIKSAM_ERROR_COUNT_PLUS_1]) != TEXT("dummy"))
    {
        ERROR_DBG_Panic(("message format error. DIKSAM_ERROR_COUNT_PLUS_1..", DIKSAM_ERROR_COUNT_PLUS_1));
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

void Error::FormatMessage(int iLine, const wchar_t* lpcwstrFormat, VString *vstrMsg, va_list ap)
{

}