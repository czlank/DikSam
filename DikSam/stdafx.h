// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�: 
#include <windows.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#ifdef _DEBUG
#pragma message("���ڱ�����԰汾...")
#else
#pragma message("��ʽ���뷢���汾...")
#endif // _DEBUG

#include <cassert>
#include <string>
#include <sstream>
#include <memory>
#include <exception>
#include <iostream>
#include <map>

#include "DikSamc.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern int  g_iCurrentThreadIndex;

#ifdef __cplusplus
}
#endif // __cplusplus
