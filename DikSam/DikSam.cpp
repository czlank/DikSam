// DikSam.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "DikSam.h"
#include "Debug.h"

#define DIKSAM_DECLARE  extern "C" __declspec (dllexport)

DIKSAM_DECLARE void DKC_Compile(int iThreadIndex)
{
    DikSam diksam(0);
}

DIKSAM_DECLARE void DKC_Run(int iThreadIndex)
{

}

DikSam::DikSam(int iThreadIndex)
    : m_Debug(new Debug)
{
    DBG_assert(1 > 0, ("1 > 0", 23));
}

DikSam::~DikSam()
{

}