#pragma once

#include "Debug.h"
#include "Memory.h"
#include "Storage.h"

class DikSam
{
// Debug
#ifdef _DEBUG
#define DBG_assert(expression, arg) ((expression) ? (void)(0) :                             \
    ((DikSam::GetClassObject(g_iCurrentThreadIndex)->GetDebug()->Set(__FILE__, __LINE__)),  \
    (DikSam::GetClassObject(g_iCurrentThreadIndex)->GetDebug()->SetExpression(#expression)),\
    DikSam::GetClassObject(g_iCurrentThreadIndex)->GetDebug()->Assert arg))

#define DBG_panic(arg)  (DikSam::GetClassObject(g_iCurrentThreadIndex)->GetDebug()->Set(__FILE__, __LINE__),  \
    DikSam::GetClassObject(g_iCurrentThreadIndex)->GetDebug()->Panic arg)
#else
#define DBG_assert(expression, arg)     ((void)0)
#define DBG_panic(arg)                  ((void)0)
#endif // _DEBUG

// Memory
#define MEM_malloc(size)                                                                                            \
    do                                                                                                              \
    {                                                                                                               \
        std::string strFileName(__FILE__);                                                                          \
        size_t npos = strFileName.find_last_of('\\');                                                               \
        assert(npos != std::string::npos);                                                                          \
        strFileName = strFileName.substr(npos + 1, strFileName.length() - (npos + 1));                              \
        DikSam::GetClassObject(g_iCurrentThreadIndex)->GetMemory()->Malloc(strFileName.c_str(), __LINE__, size);    \
    } while (0)

#define MEM_realloc(ptr, size)                                                                                          \
    do                                                                                                                  \
    {                                                                                                                   \
        std::string strFileName(__FILE__);                                                                              \
        size_t npos = strFileName.find_last_of('\\');                                                                   \
        assert(npos != std::string::npos);                                                                              \
        strFileName = strFileName.substr(npos + 1, strFileName.length() - (npos + 1));                                  \
        DikSam::GetClassObject(g_iCurrentThreadIndex)->GetMemory()->Realloc(strFileName.c_str(), __LINE__, ptr, size);  \
    } while (0)

#define MEM_strdup(str)                                                                                         \
    do                                                                                                          \
    {                                                                                                           \
        std::string strFileName(__FILE__);                                                                      \
        size_t npos = strFileName.find_last_of('\\');                                                           \
        assert(npos != std::string::npos);                                                                      \
        strFileName = strFileName.substr(npos + 1, strFileName.length() - (npos + 1));                          \
        DikSam::GetClassObject(g_iCurrentThreadIndex)->GetMemory()->StrDUP(strFileName.c_str(), __LINE__, str); \
    } while (0)

#define MEM_free(ptr)           (DikSam::GetClassObject(g_iCurrentThreadIndex)->GetMemory()->Free(ptr))

#ifdef _DEBUG
#define MEM_dump_blocks(out)    (DikSam::GetClassObject(g_iCurrentThreadIndex)->GetMemory()->DumpBlocks(out))
#define MEM_check_block(p)      (DikSam::GetClassObject(g_iCurrentThreadIndex)->GetMemory()->CheckBlock(p))
#define MEM_check_all_blocks()  (DikSam::GetClassObject(g_iCurrentThreadIndex)->GetMemory()->CheckAllBlocks())
#else
#define MEM_dump_blocks(out)    ((void)0)
#define MEM_check_block(p)      ((void)0)
#define MEM_check_all_blocks()  ((void)0)
#endif // _DEBUG

// Storage
#define MEM_open_storage(page_size)                                                                                     \
    do                                                                                                                  \
    {                                                                                                                   \
        std::string strFileName(__FILE__);                                                                              \
        size_t npos = strFileName.find_last_of('\\');                                                                   \
        assert(npos != std::string::npos);                                                                              \
        strFileName = strFileName.substr(npos + 1, strFileName.length() - (npos + 1));                                  \
        DikSam::GetClassObject(g_iCurrentThreadIndex)->GetStorage()->Open(strFileName.c_str(), __LINE__, page_size);    \
    } while (0)

#define MEM_storage_malloc(storage, size)                                                                                   \
    do                                                                                                                      \
    {                                                                                                                       \
        std::string strFileName(__FILE__);                                                                                  \
        size_t npos = strFileName.find_last_of('\\');                                                                       \
        assert(npos != std::string::npos);                                                                                  \
        strFileName = strFileName.substr(npos + 1, strFileName.length() - (npos + 1));                                      \
        DikSam::GetClassObject(g_iCurrentThreadIndex)->GetStorage()->Malloc(strFileName.c_str(), __LINE__, storage, size);  \
    } while (0)

#define MEM_dispose_storage(storage) (DikSam::GetClassObject(g_iCurrentThreadIndex)->GetStorage()->Dispose(storage))

public:
    DikSam(int iThreadIndex);
    ~DikSam();

    static DikSam* GetClassObject(int iThreadIndex);

    Debug* GetDebug() { return m_Debug.get(); }
    Memory* GetMemory() { return m_Memory.get(); }
    Storage* GetStorage() { return m_Storage.get(); }

private:
    static std::map<int, std::auto_ptr<DikSam>> m_DikSamObject;

    int                     m_iThreadIndex;
    std::auto_ptr<Debug>    m_Debug;
    std::auto_ptr<Memory>   m_Memory;
    std::auto_ptr<Storage>  m_Storage;
};