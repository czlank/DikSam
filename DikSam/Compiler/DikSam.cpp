// DikSam.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "DikSam.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

int  g_iCurrentThreadIndex = -1;

#ifdef __cplusplus
}
#endif // __cplusplus

std::map<int, std::auto_ptr<DikSam>> DikSam::m_DikSamObject;

DikSam::DikSam(int iThreadIndex)
    : m_iThreadIndex(iThreadIndex)
    , m_Memory(std::cout)
    , m_Storage(m_Memory)
    , m_StringLiteral(m_Memory, m_Storage, m_Util, m_Error)
    , m_Interface(m_Debug, m_Memory, m_Storage, m_Util, m_Error, m_StringLiteral, m_Create, m_iThreadIndex)
    , m_Util(m_Debug, m_Memory, m_Storage, m_Interface)
    , m_Error(m_Debug, m_Util)
    , m_Create(m_Debug, m_Memory, m_Error, m_Util, m_Interface)
{

}

DikSam::~DikSam()
{

}

DikSam* DikSam::GetClassObject(int iThreadIndex)
{
    if (nullptr == m_DikSamObject[iThreadIndex].get())
    {
        m_DikSamObject[iThreadIndex].reset(new DikSam(iThreadIndex));
    }

    return m_DikSamObject[iThreadIndex].get();
}

char** DikSam::Translate(const char* lpctstrScriptFile)
{
    std::ifstream ifs(lpctstrScriptFile);

    if (!ifs)
    {
        return nullptr;
    }

    std::string sLine;
    m_vecScriptFile.clear();

    while (std::getline(ifs, sLine))
    {
        sLine += "\n";
        m_vecScriptFile.push_back(sLine);
    }

    m_vecScriptFile.push_back("");

    m_ppLines.reset(new char*[m_vecScriptFile.size()]());
    for (size_t i = 0; i < m_vecScriptFile.size(); i++)
    {
#pragma message ("*** WARNING : force const char* cast to char* is dangerous ***")
#pragma message ("这里进行强制转换时，已经确定const char*中的内容不会改变，转换的目的仅用于能够完全编译")
        m_ppLines.get()[i] = const_cast<char*>(m_vecScriptFile[i].c_str());
    }

    return m_ppLines.get();
}