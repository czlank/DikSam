#pragma once

#include "Memory.h"
#include "Util.h"
#include "Error.h"
#include "DikSamc.h"

class StringLiteral
{
#define STRING_ALLOC_SIZE   (256)
#define STRING_UTIL_Malloc(size)        (m_Util.Malloc(__FILE__, __LINE__, size))
#define STRING_MEM_Malloc(size)         (m_Memory.Malloc(__FILE__, __LINE__, size))
#define STRING_MEM_Realloc(ptr, size)   (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))

public:
    StringLiteral(Memory& memory, Util& util, Error& error);
    ~StringLiteral();

    void Open();
    void Add(int letter);
    DVM_Char* Close();
    void Reset();
    char* CreateIdentifier(const char *lpcstrStr);

private:
    int     m_iIndex;
    int     m_iBufferSize;
    char    *m_pBuffer;
    Memory  &m_Memory;
    Util    &m_Util;
    Error   &m_Error;
};