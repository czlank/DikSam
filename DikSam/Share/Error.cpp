#include "stdafx.h"
#include "Error.h"
#include "DikSam.h"
#include "Debug.h"
#include "Util.h"

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
    ss << iLine << TEXT(" ") << vStrMessage.string;
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
        ss << ConvertPCToLineNumber(pExe, pFunc, pc) << TEXT(" ");
    }

    ss << vStrMessage.string;
    va_end(ap);

    m_Util.VStrClear(&vStrMessage);
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
    m_ErrorMessageFormat[6] = TEXT("数组元素不能标识为final。");
    m_ErrorMessageFormat[7] = TEXT("复合赋值运算符不能用于final值。");
    m_ErrorMessageFormat[8] = TEXT("函数的参数名重复($(name))。");
    m_ErrorMessageFormat[9] = TEXT("变量名$(name)重复。");
    m_ErrorMessageFormat[10] = TEXT("找不到变量或函数$(name)。");
    m_ErrorMessageFormat[11] = TEXT("$(name)是函数名，但没有函数调用的()。");
    m_ErrorMessageFormat[12] = TEXT("不能强制转型为派生类型。");
    m_ErrorMessageFormat[13] = TEXT("不能将$(src)转型为$(dest)。");
    m_ErrorMessageFormat[14] = TEXT("算数运算符的操作数类型不正确。");
    m_ErrorMessageFormat[15] = TEXT("比较运算符的操作数类型不正确。");
    m_ErrorMessageFormat[16] = TEXT("逻辑and/or运算符的操作数类型不正确。");
    m_ErrorMessageFormat[17] = TEXT("减法运算符的操作数类型不正确。");
    m_ErrorMessageFormat[18] = TEXT("逻辑非运算符的操作数类型不正确。");
    m_ErrorMessageFormat[19] = TEXT("自增/自减运算符的操作数类型不正确。");
    m_ErrorMessageFormat[20] = TEXT("函数调用运算符的操作数不是函数名。");
    m_ErrorMessageFormat[21] = TEXT("找不到函数$(name)。");
    m_ErrorMessageFormat[22] = TEXT("函数的参数数量错误。");
    m_ErrorMessageFormat[23] = TEXT("赋值运算符的左边不是一个左值。");
    m_ErrorMessageFormat[24] = TEXT("标签$(label)不存在。");
    m_ErrorMessageFormat[25] = TEXT("数组字面量必须至少有一个元素。");
    m_ErrorMessageFormat[26] = TEXT("下标运算符[]的左边不是数组类型。");
    m_ErrorMessageFormat[27] = TEXT("数组的下标不是int类型。");
    m_ErrorMessageFormat[28] = TEXT("数组的大小不是int类型。");
    m_ErrorMessageFormat[29] = TEXT("整数值不能被0除。");
    m_ErrorMessageFormat[30] = TEXT("dummy");
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