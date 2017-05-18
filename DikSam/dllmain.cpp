// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
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

    std::ifstream ifs(lpctstrScriptFile);

    if (!ifs)
    {
        return 0;
    }

    std::vector<std::string> vecScriptFile;
    std::string sLine;

    while (std::getline(ifs, sLine))
    {
        sLine += "\n";
        vecScriptFile.push_back(sLine);
    }

    vecScriptFile.push_back("");

    std::auto_ptr<char*> ppLines(new char*[vecScriptFile.size()]());
    for (size_t i = 0; i < vecScriptFile.size(); i++)
    {
#pragma message ("*** WARNING : force const char* cast to char* is dangerous ***")
#pragma message ("�������ǿ��ת��ʱ���Ѿ�ȷ��const char*�е����ݲ���ı䣬ת����Ŀ�Ľ������ܹ���ȫ����")
        ppLines.get()[i] = const_cast<char*>(vecScriptFile[i].c_str());
    }

    try
    {
        DikSam::GetClassObject(iThreadIndex)->GetInterface()->RunScript(ppLines.get());
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

	return 0;
}
