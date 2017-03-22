#include "stdafx.h"
#include "Storage.h"
#include "Memory.h"

Storage::Storage(Memory& memory)
    : m_Memory(memory)
{

}

Storage::~Storage()
{

}

MEM_Storage Storage::Open(const char *lpcstrFileName, int iLine, int iPageSize)
{
    MEM_Storage storage;

    storage = MEM_Storage(m_Memory.Malloc(lpcstrFileName, iLine, sizeof(struct MEM_Storage_tag)));
    storage->m_pPageList = nullptr;

    assert(iPageSize >= 0);

    if (iPageSize > 0)
    {
        storage->m_iCurrentPageSize = iPageSize;
    }
    else
    {
        storage->m_iCurrentPageSize = DEFAULT_PAGE_SIZE;
    }

    return storage;
}

void* Storage::Malloc(const char *lpcstrFileName, int iLine, MEM_Storage pStorage, size_t szSize)
{
    int iCellNum = ((szSize - 1) / CELL_SIZE) + 1;
    void *p = nullptr;

    if (pStorage->m_pPageList
        && (pStorage->m_pPageList->m_iUseCellNum + iCellNum < pStorage->m_pPageList->m_iCellNum))
    {
        p = &(pStorage->m_pPageList->m_Cell[pStorage->m_pPageList->m_iUseCellNum]);
        pStorage->m_pPageList->m_iUseCellNum += iCellNum;
    }
    else
    {
        int iAllocCellNum = larger(iCellNum, pStorage->m_iCurrentPageSize);
        MemoryPage *pNewPage = (MemoryPage*)m_Memory.Malloc(lpcstrFileName, iLine, sizeof(MemoryPage) + CELL_SIZE * (iAllocCellNum - 1));

        pNewPage->m_pNext = pStorage->m_pPageList;
        pNewPage->m_iCellNum = iAllocCellNum;
        pStorage->m_pPageList = pNewPage;

        p = &(pNewPage->m_Cell[0]);
        pNewPage->m_iUseCellNum = iCellNum;
    }

    return p;
}

void Storage::Dispose(MEM_Storage pStorage)
{
    MemoryPage *temp;

    while (pStorage->m_pPageList)
    {
        temp = pStorage->m_pPageList->m_pNext;
        m_Memory.Free(pStorage->m_pPageList);
        pStorage->m_pPageList = temp;
    }

    m_Memory.Free(pStorage);
}