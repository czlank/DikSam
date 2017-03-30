// DikSam.cpp : ���� DLL Ӧ�ó���ĵ���������
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
    , m_StringLiteral(m_Memory, m_Util, m_Error)
    , m_Interface(m_Debug, m_Memory, m_Storage, m_StringLiteral)
    , m_Util(m_Debug, m_Memory, m_Storage, m_Interface)
    , m_Error(m_Debug, m_Util)
    , m_Create(m_Debug, m_Error, m_Util, m_Interface)
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