#pragma once

#include "Debug.h"
#include "Memory.h"
#include "Storage.h"
#include "StringLiteral.h"
#include "Interface.h"
#include "Util.h"
#include "Error.h"
#include "Create.h"

class DikSam
{
public:
    DikSam(int iThreadIndex);
    ~DikSam();

    static DikSam* GetClassObject(int iThreadIndex);

    Debug* GetDebug() { return &m_Debug; }
    Memory* GetMemory() { return &m_Memory; }
    Storage* GetStorage() { return &m_Storage; }
    StringLiteral* GetStringLiteral() { return &m_StringLiteral; }
    Interface* GetInterface() { return &m_Interface; }
    Util* GetUtil() { return &m_Util; }
    Error* GetError() { return &m_Error; }
    Create* GetCreate() { return &m_Create; }

private:
    static std::map<int, std::auto_ptr<DikSam>> m_DikSamObject;

    int             m_iThreadIndex;
    Debug           m_Debug;
    Memory          m_Memory;
    Storage         m_Storage;
    StringLiteral   m_StringLiteral;
    Interface       m_Interface;
    Util            m_Util;
    Error           m_Error;
    Create          m_Create;
};