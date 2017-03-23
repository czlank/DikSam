#include "stdafx.h"
#include "Error.h"

Error::Error()
    : m_pMessageFormat(nullptr)
{
    InitMessageFormat();
}

Error::~Error()
{

}

int Error::InitMessageFormat()
{
    m_CompilerErrorMessageFormat[0]  = TEXT("dummy");
    m_CompilerErrorMessageFormat[1]  = TEXT("��($(token))���������﷨����");
    m_CompilerErrorMessageFormat[2]  = TEXT("����ȷ���ַ�($(bad_char))��");
    m_CompilerErrorMessageFormat[3]  = TEXT("�������ظ�($(name))��");
    m_CompilerErrorMessageFormat[4]  = TEXT("����ȷ�Ķ��ֽ��ַ���");
    m_CompilerErrorMessageFormat[5]  = TEXT("Ԥ����Ŀ��ַ�����");
    m_CompilerErrorMessageFormat[6]  = TEXT("�����Ĳ������ظ�($(name))��");
    m_CompilerErrorMessageFormat[7]  = TEXT("������$(name)�ظ���");
    m_CompilerErrorMessageFormat[8]  = TEXT("�Ҳ�����������$(name)��");
    m_CompilerErrorMessageFormat[9]  = TEXT("����ǿ��ת��Ϊ�������͡�");
    m_CompilerErrorMessageFormat[10] = TEXT("���ܽ�$(src)ת��Ϊ$(dest)��");
    m_CompilerErrorMessageFormat[11] = TEXT("����������Ĳ��������Ͳ���ȷ��");
    m_CompilerErrorMessageFormat[12] = TEXT("�Ƚ�������Ĳ��������Ͳ���ȷ��");
    m_CompilerErrorMessageFormat[13] = TEXT("�߼�and/or������Ĳ��������Ͳ���ȷ��");
    m_CompilerErrorMessageFormat[14] = TEXT("����������Ĳ��������Ͳ���ȷ��");
    m_CompilerErrorMessageFormat[15] = TEXT("�߼���������Ĳ��������Ͳ���ȷ��");
    m_CompilerErrorMessageFormat[16] = TEXT("����/�Լ�������Ĳ��������Ͳ���ȷ��");
    m_CompilerErrorMessageFormat[17] = TEXT("��������������Ĳ��������Ǻ�������");
    m_CompilerErrorMessageFormat[18] = TEXT("�Ҳ�������$(name)��");
    m_CompilerErrorMessageFormat[19] = TEXT("�����Ĳ�����������");
    m_CompilerErrorMessageFormat[20] = TEXT("��ֵ���������߲���һ����ֵ��");
    m_CompilerErrorMessageFormat[21] = TEXT("��ǩ$(label)�����ڡ�");
    m_CompilerErrorMessageFormat[22] = TEXT("dummy");

    m_DvmErrorMessageFormat[0] = TEXT("dummy");
    m_DvmErrorMessageFormat[1] = TEXT("����ȷ�Ķ��ֽ��ַ���");
    m_DvmErrorMessageFormat[2] = TEXT("�Ҳ�������$(name)��");
    m_DvmErrorMessageFormat[3] = TEXT("�ظ����庯��$(name)��");
    m_DvmErrorMessageFormat[4] = TEXT("dummy");
}