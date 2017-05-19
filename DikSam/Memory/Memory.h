#pragma once

class Memory
{
#define MARK_SIZE               (4)
#define ALIGN_SIZE              (sizeof(Align))
#define REVALUE_UP_ALIGN(val)   ((val) ? (((val) - 1) / ALIGN_SIZE + 1) : 0)
#define HEADER_ALIGN_SIZE       (REVALUE_UP_ALIGN(sizeof(HeaderStruct)))
#define INITIAL_MARK            (0xCC)
#define MARK                    (0xCD)

public:
    typedef enum
    {
        MEM_FAIL_AND_EXIT,
        MEM_FAIL_AND_RETURN
    } MEM_FailMode;

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
        Header_tag      *m_pPrev;
        Header_tag      *m_pNext;
        unsigned char   m_czMark[MARK_SIZE];
    } HeaderStruct;

    union Header_tag
    {
        HeaderStruct    m_stHeader;
        Align           m_unAlign[HEADER_ALIGN_SIZE];
    };

    typedef Header_tag Header;

    Memory(std::ostream& out);
    ~Memory();

    void SetFailMode(MEM_FailMode enFailMode) { m_FailMode = enFailMode; }

    void* Malloc(const char *lpcstrFileName, int iLine, size_t szSize);
    void* Realloc(const char *lpcstrFileName, int iLine, void *ptr, size_t szSize);
    char* StrDUP(const char *lpcstrFileName, int iLine, const char *lpcstrStr);
    void  Free(void *ptr);
    void  FreeLiteralPool();

    void DumpBlocks(std::ostream& out);
    void CheckBlock(void *p);
    void CheckAllBlocks();

private:
    void ChainBlock(Header *pHeader);
    void ReChainBlock(Header *pHeader);
    void UnChainBlock(Header *pHeader);
    void SetHeader(Header *pHeader, int iSize, const char *lpcstrFileName, int iLine);
    void SetTail(void *ptr, int iAllocSize);
    void CheckMarkSub(unsigned char *czMark, int iSize);
    void CheckMark(Header *pHeader);

    void ErrorHandler(std::ostream& out, const char *lpcstrFileName, int iLine, const char *lpcstrMsg);

private:
    std::ostream    &m_Out;
    MEM_FailMode    m_FailMode;
    Header          *m_BlockHeader;
};