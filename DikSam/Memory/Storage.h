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

// 内存页
struct MemoryPage_tag
{
    int             m_iCellNum;     // 这个页中内存块的数量
    int             m_iUseCellNum;  // 已经使用的内存块的数量
    MemoryPageList  m_pNext;        // 下一个内存页
    Cell            m_Cell[1];      // 指针，只想内存
};

// 存储器
struct MEM_Storage_tag
{
    MemoryPageList  m_pPageList;            // 内存页列表
    int             m_iCurrentPageSize;     // 当前内存页的大小，单位：字节
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