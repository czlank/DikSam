#include "stdafx.h"
#include "Util.h"

Util::Util(Storage& storage)
    : m_pCompiler(nullptr)
    , m_Storage(storage)
{

}

Util::~Util()
{

}

void* Util::Malloc(size_t szSize)
{
    assert(m_pCompiler);
    void *p = m_Storage.Malloc(__FILE__, __LINE__, m_pCompiler->compile_storage, szSize);

    return p;
}