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
    , m_Debug(new Debug)
    , m_Memory(new Memory(std::cout))
    , m_Storage(new Storage(*m_Memory.get()))
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