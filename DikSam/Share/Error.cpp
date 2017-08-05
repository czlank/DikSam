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
    m_ErrorMessageFormat[1] = TEXT("��($(token))���������﷨����");
    m_ErrorMessageFormat[2] = TEXT("����ȷ���ַ�($(bad_char))��");
    m_ErrorMessageFormat[3] = TEXT("�������ظ�($(name))��");
    m_ErrorMessageFormat[4] = TEXT("����ȷ�Ķ��ֽ��ַ���");
    m_ErrorMessageFormat[5] = TEXT("Ԥ����Ŀ��ַ�����");
    m_ErrorMessageFormat[6] = TEXT("����Ԫ�ز��ܱ�ʶΪfinal��");
    m_ErrorMessageFormat[7] = TEXT("���ϸ�ֵ�������������finalֵ��");
    m_ErrorMessageFormat[8] = TEXT("�����Ĳ������ظ�($(name))��");
    m_ErrorMessageFormat[9] = TEXT("������$(name)�ظ���");
    m_ErrorMessageFormat[10] = TEXT("�Ҳ�����������$(name)��");
    m_ErrorMessageFormat[11] = TEXT("$(name)�Ǻ���������û�к������õ�()��");
    m_ErrorMessageFormat[12] = TEXT("����ǿ��ת��Ϊ�������͡�");
    m_ErrorMessageFormat[13] = TEXT("���ܽ�$(src)ת��Ϊ$(dest)��");
    m_ErrorMessageFormat[14] = TEXT("����������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[15] = TEXT("�Ƚ�������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[16] = TEXT("�߼�and/or������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[17] = TEXT("����������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[18] = TEXT("�߼���������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[19] = TEXT("����/�Լ�������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[20] = TEXT("��������������Ĳ��������Ǻ�������");
    m_ErrorMessageFormat[21] = TEXT("�Ҳ�������$(name)��");
    m_ErrorMessageFormat[22] = TEXT("�����Ĳ�����������");
    m_ErrorMessageFormat[23] = TEXT("��ֵ���������߲���һ����ֵ��");
    m_ErrorMessageFormat[24] = TEXT("��ǩ$(label)�����ڡ�");
    m_ErrorMessageFormat[25] = TEXT("��������������������һ��Ԫ�ء�");
    m_ErrorMessageFormat[26] = TEXT("�±������[]����߲����������͡�");
    m_ErrorMessageFormat[27] = TEXT("������±겻��int���͡�");
    m_ErrorMessageFormat[28] = TEXT("����Ĵ�С����int���͡�");
    m_ErrorMessageFormat[29] = TEXT("����ֵ���ܱ�0����");
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