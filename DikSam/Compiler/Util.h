#pragma once

#include "DikSamc.h"
#include "Storage.h"

class Util
{
public:
    Util(Storage& storage);
    ~Util();

    void SetCompiler(DKC_Compiler *pCompiler) { assert(pCompiler), m_pCompiler = pCompiler; }
    DKC_Compiler* GetCompiler() const { return m_pCompiler; }

    void* Malloc(size_t szSize);

private:
    DKC_Compiler    *m_pCompiler;
    Storage         &m_Storage;
};