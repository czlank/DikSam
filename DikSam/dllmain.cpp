// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "DikSam.h"
#include "Exception.h"

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

DIKSAM_DECLARE int RunScript(int iThreadIndex, const char* lpctstrScriptFile)
{
    setlocale(LC_CTYPE, "");
    g_iCurrentThreadIndex = iThreadIndex;

    DikSam *pDikSam = DikSam::GetClassObject(g_iCurrentThreadIndex);

    if (char **ppLines = pDikSam->Translate(lpctstrScriptFile))
    {
        pDikSam->GetInterface()->RunScript(ppLines);
    }

    return 0;
}
