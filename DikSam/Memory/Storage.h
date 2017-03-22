#pragma once

class Memory;

typedef union
{
    long    l_dummy;
    double  d_dummy;
    void    *p_dummy;
} Cell;

typedef struct MemoryPage_tag   MemoryPage;
typedef MemoryPage              *MemoryPageList;

struct MemoryPage_tag
{
    int             m_iCellNum;
    int             m_iUseCellNum;
    MemoryPageList  m_pNext;
    Cell            m_Cell[1];
};

struct MEM_Storage_tag
{
    MemoryPageList  m_pPageList;
    int             m_iCurrentPageSize;
};

class Storage
{
public:
#define CELL_SIZE               (sizeof(Cell))
#define DEFAULT_PAGE_SIZE       (1024)          // cell num

    Storage(Memory& memory);
    ~Storage();

    MEM_Storage Open(const char *lpcstrFileName, int iLine, int iPageSize);
    void* Malloc(const char *lpcstrFileName, int iLine, MEM_Storage pStorage, size_t szSize);
    void Dispose(MEM_Storage pStorage);

private:
    Memory  &m_Memory;
};