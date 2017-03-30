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

DIKSAM_DECLARE void DKC_Compile(int iThreadIndex)
{
    setlocale(LC_CTYPE, "");
    g_iCurrentThreadIndex = iThreadIndex;

    char *sc[] = {
        "int int_val;\n",
        "int_val = 3;\n",
        ""
    };

    try
    {
        DikSam::GetClassObject(iThreadIndex)->GetInterface()->Compile(sc);
    }
    catch (const PanicException& e)
    {
        std::cout << e.what();
    }
    catch (const AssertException& e)
    {
        std::cout << e.what();
    }
    catch (const MemoryException& e)
    {
        std::cout << e.what();
    }
    catch (const ErrorException& e)
    {
        std::cout << e.what();
    }
}

DIKSAM_DECLARE void DKC_Run(int iThreadIndex)
{

}