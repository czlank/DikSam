#ifndef STORAGE_H
#define STORAGE_H

#include "Memory.h"

typedef union
{
    long    l_dummy;
    double  d_dummy;
    void   *p_dummy;
} Cell;

#define CELL_SIZE           (sizeof(Cell))
#define DEFAULT_PAGE_SIZE   (1024)  /* cell num */

typedef struct MemoryPage_tag MemoryPage;
typedef MemoryPage *MemoryPageList;

struct MemoryPage_tag
{
    int             cell_num;
    int             use_cell_num;
    MemoryPageList  next;
    Cell            cell[1];
};

struct MEM_Storage_tag
{
    MemoryPageList  page_list;
    int             current_page_size;
};

#define larger(a, b)    (((a) > (b)) ? (a) : (b))

#endif  /* STORAGE_H */
