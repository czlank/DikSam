#include "StdAfx.h"
#include "StringLiteral.h"
#include "DikSam.h"
#include "Memory.h"
#include "Storage.h"
#include "Util.h"
#include "Error.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern int  g_iCurrentThreadIndex;

void dkc_open_string_literal(void)
{
    DikSam::GetClassObject(g_iCurrentThreadIndex)->GetStringLiteral()->Open();
}

void dkc_add_string_literal(int letter)
{
    DikSam::GetClassObject(g_iCurrentThreadIndex)->GetStringLiteral()->Add(letter);
}

DVM_Char* dkc_close_string_literal(void)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetStringLiteral()->Close();
}

char* dkc_create_identifier(char *str)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetStringLiteral()->CreateIdentifier(str);
}

#ifdef __cplusplus
}
#endif // __cplusplus

StringLiteral::StringLiteral(Memory& memory, Storage& storage, Util& util, Error& error)
    : m_iIndex(0)
    , m_iBufferSize(0)
    , m_pBuffer(nullptr)
    , m_Memory(memory)
    , m_Storage(storage)
    , m_Util(util)
    , m_Error(error)
{

}

StringLiteral::~StringLiteral()
{

}

void StringLiteral::Open()
{
    Reset();
}

void StringLiteral::Add(int letter)
{
    if (m_iIndex == m_iBufferSize)
    {
        m_iBufferSize += STRING_ALLOC_SIZE;
        m_pBuffer = (char*)STRING_MEM_Realloc(m_pBuffer, m_iBufferSize);
    }

    m_pBuffer[m_iIndex++] = letter;
}

DVM_Char* StringLiteral::Close()
{
    Add('\0');

    size_t _Dsize = m_iIndex;
    wchar_t *_Dest = new wchar_t[_Dsize]();
    size_t iCount;
    mbstowcs_s(&iCount, _Dest, _Dsize, m_pBuffer, _Dsize);

    std::wstring str(_Dest);
    delete [] _Dest;

    DVM_Char *pNewStr = (DVM_Char*)STRING_UTIL_Malloc(sizeof(DVM_Char*)* (str.length() + 1));
    for (size_t i = 0; i < str.length(); i++)
    {
        pNewStr[i] = str[i];
    }
    pNewStr[str.length()] = L'\0';

    return pNewStr;
}

void StringLiteral::Reset()
{
    m_Memory.Free(m_pBuffer);
    m_pBuffer = nullptr;
    m_iIndex = 0;
    m_iBufferSize = 0;
}

char* StringLiteral::CreateIdentifier(const char *lpcstrStr)
{
    int iSize = lpcstrStr ? std::string(lpcstrStr).length() + 1 : 1;
    char *pNewStr = (char*)STRING_UTIL_Malloc(iSize);
    
    strcpy_s(pNewStr, iSize, lpcstrStr);
    return pNewStr;
}