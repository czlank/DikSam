// DikSam.cpp : ���� DLL Ӧ�ó���ĵ���������
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