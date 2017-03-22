#include "stdafx.h"
#include "Memory.h"
#include "Exception.h"

Memory::Memory(std::ostream& out)
    : m_Out(out)
    , m_FailMode(MEM_FAIL_AND_EXIT)
    , m_BlockHeader(nullptr)
{

}

Memory::~Memory()
{

}

void* Memory::Malloc(const char *lpcstrFileName, int iLine, size_t szSize)
{
    size_t  szAllocSize;

#ifdef _DEBUG
    szAllocSize = szSize + sizeof(Header)+MARK_SIZE;
#else
    szAllocSize = szSize;
#endif // _DEBUG

    void *ptr = new unsigned char[szAllocSize]{INITIAL_MARK};
    if (nullptr == ptr)
    {
        ErrorHandler(m_Out, lpcstrFileName, iLine, "malloc");
    }

#ifdef _DEBUG
    SetHeader((Header*)ptr, szSize, lpcstrFileName, iLine);
    SetTail(ptr, szAllocSize);
    ChainBlock((Header*)ptr);
    ptr = (char*)ptr + sizeof(Header);
#endif // _DEBUG

    return ptr;
}

void* Memory::Realloc(const char *lpcstrFileName, int iLine, void *ptr, size_t szSize)
{
    size_t  szAllocSize;
    void    *RealPtr;

#ifdef _DEBUG
    Header  stOldHeader;
    size_t  szOldSize;

    szAllocSize = szSize + sizeof(Header) + MARK_SIZE;

    if (ptr)
    {
        RealPtr = (char *)ptr - sizeof(Header);
        CheckMark((Header*)RealPtr);
        stOldHeader = *((Header*)RealPtr);
        szOldSize = stOldHeader.m_stHeader.m_iSize;
        UnChainBlock((Header*)RealPtr);
    }
    else
    {
        RealPtr = nullptr;
        szOldSize = 0;
    }
#else
    szAllocSize = szSize;
    RealPtr = ptr;
#endif // _DEBUG

    if (RealPtr)
    {
        delete[] RealPtr;
        RealPtr = nullptr;
    }

    void *NewPtr = new unsigned char[szAllocSize]{INITIAL_MARK};

    if (nullptr == NewPtr)
    {
        if (nullptr == ptr)
        {
            ErrorHandler(m_Out, lpcstrFileName, iLine, "realloc(malloc)");
        }
        else
        {
            ErrorHandler(m_Out, lpcstrFileName, iLine, "realloc");
        }
    }

#ifdef _DEBUG
    if (ptr)
    {
        *((Header*)NewPtr) = stOldHeader;
        ((Header*)NewPtr)->m_stHeader.m_iSize = szSize;
        ReChainBlock((Header*)NewPtr);
        SetTail(NewPtr, szAllocSize);
    }
    else
    {
        SetHeader((Header*)NewPtr, szSize, lpcstrFileName, iLine);
        SetTail(NewPtr, szAllocSize);
        ChainBlock((Header*)NewPtr);
    }

    NewPtr = (char*)NewPtr + sizeof(Header);
#endif // _DEBUG

    return NewPtr;
}

char* Memory::StrDUP(const char *lpcstrFileName, int iLine, const char *lpcstrStr)
{
    size_t szAllocSize;
    int iSize = lpcstrStr ? (std::string(lpcstrStr).length() + 1) : 1;

#ifdef _DEBUG
    szAllocSize = iSize + sizeof(Header) + MARK_SIZE;
#else
    szAllocSize = iSize;
#endif // _DEBUG

    char *ptr = new char[szAllocSize]{};
    if (nullptr == ptr)
    {
        ErrorHandler(m_Out, lpcstrFileName, iLine, "strdup");
    }

#ifdef _DEBUG
    memset(ptr, INITIAL_MARK, szAllocSize);
    SetHeader((Header*)ptr, iSize, lpcstrFileName, iLine);
    SetTail(ptr, szAllocSize);
    ChainBlock((Header*)ptr);
    ptr = (char*)ptr + sizeof(Header);
#endif // _DEBUG

    strcpy_s(ptr, szAllocSize, lpcstrStr);

    return ptr;
}

void Memory::Free(void *ptr)
{
    if (nullptr == ptr)
        return;

#ifdef _DEBUG
    void *RealPtr = (char*)ptr - sizeof(Header);
    CheckMark((Header*)RealPtr);
    
    int iSize = ((Header*)RealPtr)->m_stHeader.m_iSize;
    UnChainBlock((Header*)RealPtr);
#else
    void *RealPtr = ptr;
#endif // _DEBUG

    delete[] RealPtr;
}

void Memory::DumpBlocks(std::ostream& out)
{
#ifdef _DEBUG
    int iCounter = 0;

    for (Header *pos = m_BlockHeader; pos; pos = pos->m_stHeader.m_pNext)
    {
        CheckMark(pos);
        out << "[" << iCounter << "]" << std::hex << int((char*)pos + sizeof(Header)) << std::dec << "********************" << std::endl
            << pos->m_stHeader.m_lpcstrFileName << " line " << pos->m_stHeader.m_iLine << " size.." << pos->m_stHeader.m_iSize << std::endl;
        
        out << "====" << std::endl;
        for (int i = 0; i < pos->m_stHeader.m_iSize;)
        {
            char ch = *((char*)pos + sizeof(Header) + i);
            if (ch >= 0 && isprint(ch))
            {
                out << ch;
            }
            else
            {
                out << '.';
            }

            if (0 == ++i % 16)
                out << std::endl;
        }
        out << std::endl << "====" << std::endl;

        iCounter++;
    }
#endif // _DEBUG
}

void Memory::CheckBlock(void *p)
{
#ifdef _DEBUG
    void *RealPtr = ((char*)p) - sizeof(Header);

    CheckMark((Header*)RealPtr);
#endif // _DEBUG
}

void Memory::CheckAllBlocks()
{
#ifdef _DEBUG
    Header  *pos;

    for (pos = m_BlockHeader; pos; pos = pos->m_stHeader.m_pNext)
    {
        CheckBlock(pos);
    }
#endif // _DEBUG
}

#ifdef _DEBUG
void Memory::ChainBlock(Header *pHeader)
{
    if (m_BlockHeader)
    {
        m_BlockHeader->m_stHeader.m_pPrev = pHeader;
    }

    pHeader->m_stHeader.m_pPrev = nullptr;
    pHeader->m_stHeader.m_pNext = m_BlockHeader;
    m_BlockHeader = pHeader;
}

void Memory::ReChainBlock(Header *pHeader)
{
    if (pHeader->m_stHeader.m_pPrev)
    {
        pHeader->m_stHeader.m_pPrev->m_stHeader.m_pNext = pHeader;
    }
    else
    {
        m_BlockHeader = pHeader;
    }

    if (pHeader->m_stHeader.m_pNext)
    {
        pHeader->m_stHeader.m_pNext->m_stHeader.m_pPrev = pHeader;
    }
}

void Memory::UnChainBlock(Header *pHeader)
{
    if (pHeader->m_stHeader.m_pPrev)
    {
        pHeader->m_stHeader.m_pPrev->m_stHeader.m_pNext = pHeader->m_stHeader.m_pNext;
    }
    else
    {
        m_BlockHeader = pHeader->m_stHeader.m_pNext;
    }

    if (pHeader->m_stHeader.m_pNext)
    {
        pHeader->m_stHeader.m_pNext->m_stHeader.m_pPrev = pHeader->m_stHeader.m_pPrev;
    }
}

void Memory::SetHeader(Header *pHeader, int iSize, const char *lpcstrFileName, int iLine)
{
    pHeader->m_stHeader.m_iSize = iSize;
    pHeader->m_stHeader.m_lpcstrFileName = lpcstrFileName;
    pHeader->m_stHeader.m_iLine = iLine;

    memset(pHeader->m_stHeader.m_czMark, MARK, (char*)&pHeader[1] - (char*)pHeader->m_stHeader.m_czMark);
}

void Memory::SetTail(void *ptr, int iAllocSize)
{
    char *tail;
    tail = ((char*)ptr) + iAllocSize - MARK_SIZE;
    
    memset(tail, MARK, MARK_SIZE);
}

void Memory::CheckMarkSub(unsigned char *czMark, int iSize)
{
    for (int i = 0; i < iSize; i++)
    {
        if (czMark[i] != MARK)
        {
            throw MemoryException("bad mark");
        }
    }
}

void Memory::CheckMark(Header *pHeader)
{
    unsigned char   *tail;

    CheckMarkSub(pHeader->m_stHeader.m_czMark, (char*)&pHeader[1] - (char*)pHeader->m_stHeader.m_czMark);
    tail = ((unsigned char*)pHeader) + pHeader->m_stHeader.m_iSize + sizeof(pHeader);
    CheckMarkSub(tail, MARK_SIZE);
}
#endif // _DEBUG

void Memory::ErrorHandler(std::ostream& out, const char *lpcstrFileName, int iLine, const char *lpcstrMsg)
{
    out << "MEM:" << lpcstrMsg << " failed in " << lpcstrFileName << " at " << iLine;

    std::stringstream ss;
    ss << "MEM:" << lpcstrMsg << " failed in " << lpcstrFileName << " at " << iLine;

    if (MEM_FAIL_AND_EXIT == m_FailMode)
    {
        throw MemoryException(ss.str().c_str());
    }
}