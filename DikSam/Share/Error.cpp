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
    m_ErrorMessageFormat[1] = TEXT("��($(token))���������﷨����");
    m_ErrorMessageFormat[2] = TEXT("����ȷ���ַ�($(bad_char))��");
    m_ErrorMessageFormat[3] = TEXT("�������ظ�($(name))��");
    m_ErrorMessageFormat[4] = TEXT("����ȷ�Ķ��ֽ��ַ���");
    m_ErrorMessageFormat[5] = TEXT("Ԥ����Ŀ��ַ�����");
    m_ErrorMessageFormat[6] = TEXT("�����Ĳ������ظ�($(name))��");
    m_ErrorMessageFormat[7] = TEXT("������$(name)�ظ���");
    m_ErrorMessageFormat[8] = TEXT("�Ҳ�����������$(name)��");
    m_ErrorMessageFormat[9] = TEXT("����ǿ��ת��Ϊ�������͡�");
    m_ErrorMessageFormat[10] = TEXT("���ܽ�$(src)ת��Ϊ$(dest)��");
    m_ErrorMessageFormat[11] = TEXT("����������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[12] = TEXT("�Ƚ�������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[13] = TEXT("�߼�and/or������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[14] = TEXT("����������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[15] = TEXT("�߼���������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[16] = TEXT("����/�Լ�������Ĳ��������Ͳ���ȷ��");
    m_ErrorMessageFormat[17] = TEXT("��������������Ĳ��������Ǻ�������");
    m_ErrorMessageFormat[18] = TEXT("�Ҳ�������$(name)��");
    m_ErrorMessageFormat[19] = TEXT("�����Ĳ�����������");
    m_ErrorMessageFormat[20] = TEXT("��ֵ���������߲���һ����ֵ��");
    m_ErrorMessageFormat[21] = TEXT("��ǩ$(label)�����ڡ�");
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