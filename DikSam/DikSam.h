#pragma once

#include "Debug.h"
#include "Memory.h"
#include "Storage.h"
#include "Util.h"

class DikSam
{
public:
    DikSam(int iThreadIndex);
    ~DikSam();

    static DikSam* GetClassObject(int iThreadIndex);

    Debug* GetDebug() { return &m_Debug; }
    Memory* GetMemory() { return &m_Memory; }
    Storage* GetStorage() { return &m_Storage; }
    Util* GetUtil() { return &m_Util; }

private:
    static std::map<int, std::auto_ptr<DikSam>> m_DikSamObject;

    int         m_iThreadIndex;
    Debug       m_Debug;
    Memory      m_Memory;
    Storage     m_Storage;
    Util        m_Util;
};