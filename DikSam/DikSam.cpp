// DikSam.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "DikSam.h"

DikSam::DikSam(int iThreadIndex)
    : m_Debug(new Debug)
    , m_Memory(new Memory(std::cout))
{
}

DikSam::~DikSam()
{

}