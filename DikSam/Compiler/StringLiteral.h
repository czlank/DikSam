#pragma once

#include "DikSamc.h"

class Memory;
class Util;
class Storage;
class Error;

class StringLiteral
{
public:
    StringLiteral(Memory& memory, Storage& storage, Util& util, Error& error);
    ~StringLiteral();

    void Open();
    void Add(int letter);
    DVM_Char* CloseString();
    int CloseCharacter();
    void Reset();
    char* CreateIdentifier(const char *lpcstrStr);

private:
    int         m_iIndex;
    int         m_iBufferSize;
    char        *m_pBuffer;
    Memory      &m_Memory;
    Storage     &m_Storage;
    Util        &m_Util;
    Error       &m_Error;
};