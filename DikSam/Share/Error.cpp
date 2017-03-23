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
    m_CompilerErrorMessageFormat[1]  = TEXT("在($(token))附近发生语法错误。");
    m_CompilerErrorMessageFormat[2]  = TEXT("不正确的字符($(bad_char))。");
    m_CompilerErrorMessageFormat[3]  = TEXT("函数名重复($(name))。");
    m_CompilerErrorMessageFormat[4]  = TEXT("不正确的多字节字符。");
    m_CompilerErrorMessageFormat[5]  = TEXT("预期外的宽字符串。");
    m_CompilerErrorMessageFormat[6]  = TEXT("函数的参数名重复($(name))。");
    m_CompilerErrorMessageFormat[7]  = TEXT("变量名$(name)重复。");
    m_CompilerErrorMessageFormat[8]  = TEXT("找不到变量或函数$(name)。");
    m_CompilerErrorMessageFormat[9]  = TEXT("不能强制转型为派生类型。");
    m_CompilerErrorMessageFormat[10] = TEXT("不能将$(src)转型为$(dest)。");
    m_CompilerErrorMessageFormat[11] = TEXT("算数运算符的操作数类型不正确。");
    m_CompilerErrorMessageFormat[12] = TEXT("比较运算符的操作数类型不正确。");
    m_CompilerErrorMessageFormat[13] = TEXT("逻辑and/or运算符的操作数类型不正确。");
    m_CompilerErrorMessageFormat[14] = TEXT("减法运算符的操作数类型不正确。");
    m_CompilerErrorMessageFormat[15] = TEXT("逻辑非运算符的操作数类型不正确。");
    m_CompilerErrorMessageFormat[16] = TEXT("自增/自减运算符的操作数类型不正确。");
    m_CompilerErrorMessageFormat[17] = TEXT("函数调用运算符的操作数不是函数名。");
    m_CompilerErrorMessageFormat[18] = TEXT("找不到函数$(name)。");
    m_CompilerErrorMessageFormat[19] = TEXT("函数的参数数量错误。");
    m_CompilerErrorMessageFormat[20] = TEXT("赋值运算符的左边不是一个左值。");
    m_CompilerErrorMessageFormat[21] = TEXT("标签$(label)不存在。");
    m_CompilerErrorMessageFormat[22] = TEXT("dummy");

    m_DvmErrorMessageFormat[0] = TEXT("dummy");
    m_DvmErrorMessageFormat[1] = TEXT("不正确的多字节字符。");
    m_DvmErrorMessageFormat[2] = TEXT("找不到函数$(name)。");
    m_DvmErrorMessageFormat[3] = TEXT("重复定义函数$(name)。");
    m_DvmErrorMessageFormat[4] = TEXT("dummy");
}