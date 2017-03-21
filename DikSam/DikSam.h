#pragma once

#include "Debug.h"
#include "Memory.h"

class DikSam
{
// Debug
#ifdef _DEBUG
#define DBG_assert(expression, arg) ((expression) ? (void)(0) : \
    ((m_Debug.get()->Set(__FILE__, __LINE__)), (m_Debug.get()->SetExpression(#expression)), \
    m_Debug.get()->Assert arg))
#define DBG_panic(arg)  (m_Debug.get()->Set(__FILE__, __LINE__), m_Debug.get()->Panic arg)
#else
#define DBG_assert(expression, arg)     ((void)0)
#define DBG_panic(arg)                  ((void)0)
#endif // _DEBUG

// Memory
#define MEM_malloc(size)                                                                \
    do                                                                                  \
    {                                                                                   \
        std::string strFileName(__FILE__);                                              \
        size_t npos = strFileName.find_last_of('\\');                                   \
        assert(npos != std::string::npos);                                              \
        strFileName = strFileName.substr(npos + 1, strFileName.length() - (npos + 1));  \
        m_Memory.get()->Malloc(strFileName.c_str(), __LINE__, size);                    \
    } while (0)

#define MEM_realloc(ptr, size)                                                          \
    do                                                                                  \
    {                                                                                   \
        std::string strFileName(__FILE__);                                              \
        size_t npos = strFileName.find_last_of('\\');                                   \
        assert(npos != std::string::npos);                                              \
        strFileName = strFileName.substr(npos + 1, strFileName.length() - (npos + 1));  \
        m_Memory.get()->Realloc(strFileName.c_str(), __LINE__, ptr, size);              \
    } while (0)

#define MEM_strdup(str)                                                                 \
    do                                                                                  \
    {                                                                                   \
        std::string strFileName(__FILE__);                                              \
        size_t npos = strFileName.find_last_of('\\');                                   \
        assert(npos != std::string::npos);                                              \
        strFileName = strFileName.substr(npos + 1, strFileName.length() - (npos + 1));  \
        m_Memory.get()->StrDUP(strFileName.c_str(), __LINE__, str);                     \
    } while (0)

#define MEM_free(ptr)           (m_Memory.get()->Free(ptr))
#define MEM_dump_blocks(out)    (m_Memory.get()->DumpBlocks(out))
#define MEM_check_block(p)      (m_Memory.get()->CheckBlock(p))
#define MEM_check_all_blocks()  (m_Memory.get()->CheckAllBlocks())

public:
    DikSam(int iThreadIndex);
    ~DikSam();

private:
    std::auto_ptr<Debug>    m_Debug;
    std::auto_ptr<Memory>   m_Memory;
};