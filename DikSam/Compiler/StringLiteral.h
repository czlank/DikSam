#pragma once

#include "DikSamc.h"

class Memory;
class Util;
class Storage;
class Error;
class Interface;

class StringLiteral
{
public:
    StringLiteral(Memory& memory, Storage& storage, Util& util, Error& error, Interface& interfaceRef);
    ~StringLiteral();

    void Open();
    void Add(int letter);
    DVM_Char* CloseString();
    int CloseCharacter();
    void Reset();
    char* CreateIdentifier(const char *lpcstrStr);

private:
    Memory      &m_Memory;
    Storage     &m_Storage;
    Util        &m_Util;
    Error       &m_Error;
    Interface   &m_Interface;

    int         m_iIndex;
    int         m_iBufferSize;
    char        *m_pBuffer;
};