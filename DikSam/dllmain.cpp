// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "DikSam.h"

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#define DIKSAM_DECLARE  extern "C" __declspec (dllexport)

DIKSAM_DECLARE void DKC_Compile(int iThreadIndex)
{
    g_iCurrentThreadIndex = iThreadIndex;
}

DIKSAM_DECLARE void DKC_Run(int iThreadIndex)
{

}