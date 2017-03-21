#pragma once

#include "MEM.h"

class Memory
{
public:
    Memory(std::ostream& out);
    ~Memory();

#define MARK_SIZE               (4)
#define ALIGN_SIZE              (sizeof(Align))
#define REVALUE_UP_ALIGN(val)   ((val) ? (((val) - 1) / ALIGN_SIZE + 1) : 0)
#define HEADER_ALIGN_SIZE       (REVALUE_UP_ALIGN(sizeof(HeaderStruct)))
#define INITIAL_MARK            (0xCC)
#define MARK                    (0xCD)

    typedef union
    {
        long    l_dummy;
        double  d_dummy;
        void    *p_dummy;
    } Align;

    union Header_tag;

    typedef struct 
    {
        int             m_iSize;
        const char      *m_lpcstrFileName;
        int             m_iLine;
        Header_tag      *m_stPrev;
        Header_tag      *m_stNext;
        unsigned char   m_czMark[MARK_SIZE];
    } HeaderStruct;

    union Header_tag
    {
        HeaderStruct    m_stHeader;
        Align           m_unAlign[HEADER_ALIGN_SIZE];
    };

    typedef Header_tag Header;

    void SetFailMode(MEM_FailMode enFailMode) { m_FailMode = enFailMode; }

    void* Malloc(const char *lpcstrFileName, int iLine, size_t szSize);
    void* Realloc(const char *lpcstrFileName, int iLine, void *ptr, size_t szSize);
    char* StrDUP(const char *lpcstrFileName, int iLine, const char *lpcstrStr);
    void  Free(void *ptr);

    void DumpBlocks(std::ostream& out);
    void CheckBlock(void *p);
    void CheckAllBlocks();

private:
#ifdef _DEBUG
    void ChainBlock(Header *pHeader);
    void ReChainBlock(Header *pHeader);
    void UnChainBlock(Header *pHeader);
    void SetHeader(Header *pHeader, int iSize, const char *lpcstrFileName, int iLine);
    void SetTail(void *ptr, int iAllocSize);
    void CheckMarkSub(unsigned char *czMark, int iSize);
    void CheckMark(Header *pHeader);
#endif // _DEBUG

    void ErrorHandler(std::ostream& out, const char *lpcstrFileName, int iLine, const char *lpcstrMsg);

private:
    std::ostream    &m_Out;
    MEM_FailMode    m_FailMode;
    Header          *m_BlockHeader;
};