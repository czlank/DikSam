#pragma once

#include "DikSamc.h"

class FixTree
{
public:
    FixTree();
    ~FixTree();

    void Fix(DKC_Compiler *pCompiler);
};