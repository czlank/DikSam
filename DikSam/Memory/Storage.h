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

// �ڴ�ҳ
struct MemoryPage_tag
{
    int             m_iCellNum;     // ���ҳ���ڴ�������
    int             m_iUseCellNum;  // �Ѿ�ʹ�õ��ڴ�������
    MemoryPageList  m_pNext;        // ��һ���ڴ�ҳ
    Cell            m_Cell[1];      // ָ�룬ֻ���ڴ�
};

// �洢��
struct MEM_Storage_tag
{
    MemoryPageList  m_pPageList;            // �ڴ�ҳ�б�
    int             m_iCurrentPageSize;     // ��ǰ�ڴ�ҳ�Ĵ�С����λ���ֽ�
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