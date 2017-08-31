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
    size_t  szAllocSize = szSize + sizeof(Header) + MARK_SIZE;

    void *ptr = new unsigned char[szAllocSize]();
    if (nullptr == ptr)
    {
        ErrorHandler(m_Out, lpcstrFileName, iLine, "malloc");
    }

    memset(ptr, INITIAL_MARK, szAllocSize);
    SetHeader((Header*)ptr, szSize, lpcstrFileName, iLine);
    SetTail(ptr, szAllocSize);
    ChainBlock((Header*)ptr);
    ptr = (char*)ptr + sizeof(Header);

    return ptr;
}

void* Memory::Realloc(const char *lpcstrFileName, int iLine, void *ptr, size_t szSize)
{
    size_t  szAllocSize;
    void    *RealPtr;

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

    void *pNewPtr = new unsigned char[szAllocSize]();

    if (nullptr == pNewPtr)
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

    if (ptr)
    {
        if (szOldSize >= szSize)
        {
            memcpy((char*)pNewPtr + sizeof(Header), ptr, szSize);
        }
        else
        {
            memcpy((char*)pNewPtr + sizeof(Header), ptr, szOldSize);
        }

        delete [] RealPtr;
        RealPtr = nullptr;
    }

    if (ptr)
    {
        *((Header*)pNewPtr) = stOldHeader;
        ((Header*)pNewPtr)->m_stHeader.m_iSize = szSize;
        ReChainBlock((Header*)pNewPtr);
        SetTail(pNewPtr, szAllocSize);
    }
    else
    {
        SetHeader((Header*)pNewPtr, szSize, lpcstrFileName, iLine);
        SetTail(pNewPtr, szAllocSize);
        ChainBlock((Header*)pNewPtr);
    }

    pNewPtr = (char*)pNewPtr + sizeof(Header);

    if (szSize > szOldSize)
    {
        memset((char *)pNewPtr + szOldSize, INITIAL_MARK, szSize - szOldSize);
    }

    return pNewPtr;
}

char* Memory::StrDUP(const char *lpcstrFileName, int iLine, const char *lpcstrStr)
{
    int iSize = lpcstrStr ? (std::string(lpcstrStr).length() + 1) : 1;
    size_t szAllocSize = iSize + sizeof(Header) + MARK_SIZE;

    char *ptr = new char[szAllocSize]();
    if (nullptr == ptr)
    {
        ErrorHandler(m_Out, lpcstrFileName, iLine, "strdup");
    }

    memset(ptr, INITIAL_MARK, szAllocSize);
    SetHeader((Header*)ptr, iSize, lpcstrFileName, iLine);
    SetTail(ptr, szAllocSize);
    ChainBlock((Header*)ptr);
    ptr = (char*)ptr + sizeof(Header);

    strcpy_s(ptr, iSize, lpcstrStr);

    return ptr;
}

void Memory::Free(void *ptr)
{
    if (nullptr == ptr)
        return;

    void *RealPtr = (char*)ptr - sizeof(Header);
    CheckMark((Header*)RealPtr);
    UnChainBlock((Header*)RealPtr);

    delete [] RealPtr;
}

void Memory::FreeLiteralPool()
{
    for (Header *pos = m_BlockHeader; pos;)
    {
        CheckMark(pos);
        UnChainBlock(pos);

        void *ptr = pos;
        pos = pos->m_stHeader.m_pNext;
        delete [] ptr;
    }
}

void Memory::DumpBlocks(std::ostream& out)
{
    int iCounter = 0;

    if (m_BlockHeader)
    {
        out << "################################################################################" << std::endl;
    }

    for (Header *pos = m_BlockHeader; pos; pos = pos->m_stHeader.m_pNext)
    {
        CheckMark(pos);
        out << "[" << iCounter << "]0x" << std::hex << int((char*)pos + sizeof(Header)) << std::dec << "********************" << std::endl
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

            i++;
            if (0 == i % 64)
                out << std::endl;
            else if (0 == i % 16)
                out << "  ";
            else if (0 == i % 8)
                out << " ";
        }
        out << std::endl << "====" << std::endl;

        iCounter++;
    }

    if (m_BlockHeader)
    {
        out << "################################################################################" << std::endl << std::endl;
    }
}

void Memory::CheckBlock(void *p)
{
    void *RealPtr = ((char*)p) - sizeof(Header);

    CheckMark((Header*)RealPtr);
}

void Memory::CheckAllBlocks()
{
    for (Header *pos = m_BlockHeader; pos; pos = pos->m_stHeader.m_pNext)
    {
        CheckMark(pos);
    }
}

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
    std::string strFileName(lpcstrFileName);
    size_t npos = strFileName.find_last_of('\\');
    assert(npos != std::string::npos);

    pHeader->m_stHeader.m_iSize = iSize;
    pHeader->m_stHeader.m_lpcstrFileName = lpcstrFileName + npos + 1;
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
    tail = ((unsigned char*)pHeader) + pHeader->m_stHeader.m_iSize + sizeof(Header);
    CheckMarkSub(tail, MARK_SIZE);
}

void Memory::ErrorHandler(std::ostream& out, const char *lpcstrFileName, int iLine, const char *lpcstrMsg)
{
    std::string strFileName(lpcstrFileName);
    size_t npos = strFileName.find_last_of('\\');
    assert(npos != std::string::npos);
    strFileName = strFileName.substr(npos + 1, strFileName.length() - (npos + 1));

    out << "MEM:" << lpcstrMsg << " failed in " << strFileName << " at " << iLine;

    std::stringstream ss;
    ss << "MEM:" << lpcstrMsg << " failed in " << strFileName << " at " << iLine;

    if (MEM_FAIL_AND_EXIT == m_FailMode)
    {
        throw MemoryException(ss.str().c_str());
    }
}