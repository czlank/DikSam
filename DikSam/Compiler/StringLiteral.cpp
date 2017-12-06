#include "StdAfx.h"
#include "StringLiteral.h"
#include "DikSam.h"
#include "Memory.h"
#include "Storage.h"
#include "Util.h"
#include "Error.h"
#include "Interface.h"

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
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetStringLiteral()->CloseString();
}

int dkc_close_character_literal(void)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetStringLiteral()->CloseCharacter();
}

char* dkc_create_identifier(char *str)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetStringLiteral()->CreateIdentifier(str);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#define STRING_ALLOC_SIZE                       (256)

#ifdef MEM_malloc
#undef MEM_malloc
#endif
#define MEM_malloc(size)                    (m_Memory.Malloc(__FILE__, __LINE__, size))

#ifdef MEM_realloc
#undef MEM_realloc
#endif
#define MEM_realloc(ptr, size)              (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))

#ifdef dkc_malloc
#undef dkc_malloc
#endif
#define dkc_malloc(size)                    (m_Util.Malloc(__FILE__, __LINE__, size))

StringLiteral::StringLiteral(Memory& memory, Storage& storage, Util& util, Error& error, Interface& interfaceRef)
    : m_Memory(memory)
    , m_Storage(storage)
    , m_Util(util)
    , m_Error(error)
    , m_Interface(interfaceRef)
    , m_iIndex(0)
    , m_iBufferSize(0)
    , m_pBuffer(nullptr)
{
    Reset();
}

StringLiteral::~StringLiteral()
{

}

void StringLiteral::Open()
{
    
}

void StringLiteral::Add(int letter)
{
    if (m_iIndex == m_iBufferSize)
    {
        m_iBufferSize += STRING_ALLOC_SIZE;
        m_pBuffer = (char*)MEM_realloc(m_pBuffer, m_iBufferSize);
    }

    m_pBuffer[m_iIndex++] = letter;
}

DVM_Char* StringLiteral::CloseString()
{
    Add('\0');

    size_t _Dsize = m_iIndex;
    wchar_t *_Dest = new wchar_t[_Dsize]();
    size_t iCount;
    mbstowcs_s(&iCount, _Dest, _Dsize, m_pBuffer, _Dsize);

    std::wstring str(_Dest);
    delete [] _Dest;

    DVM_Char *pNewStr = (DVM_Char*)MEM_malloc(sizeof(DVM_Char) * (str.length() + 1));
    for (size_t i = 0; i < str.length(); i++)
    {
        pNewStr[i] = str[i];
    }
    pNewStr[str.length()] = L'\0';

    Reset();

    return pNewStr;
}

int StringLiteral::CloseCharacter()
{
    DVM_Char buf[16]{};

    Add('\0');

    size_t _Dsize = m_iIndex;
    wchar_t *_Dest = new wchar_t[_Dsize]();
    size_t iCount;
    errno_t err = mbstowcs_s(&iCount, _Dest, _Dsize, m_pBuffer, _Dsize);

    std::wstring str(_Dest);
    delete[] _Dest;

    if (!err)
    {
        m_Error.CompileError(m_Interface.GetCurrentCompiler()->current_line_number,
            BAD_MULTIBYTE_CHARACTER_ERR, MESSAGE_ARGUMENT_END);
    }
    else if (iCount > 1)
    {
        m_Error.CompileError(m_Interface.GetCurrentCompiler()->current_line_number,
            TOO_LONG_CHARACTER_LITERAL_ERR, MESSAGE_ARGUMENT_END);
    }

    if (!str.empty())
    {
        buf[0] = str[0];
    }

    Reset();

    return buf[0];
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
    char *pNewStr = (char*)dkc_malloc(iSize);
    
    strcpy_s(pNewStr, iSize, lpcstrStr);
    return pNewStr;
}