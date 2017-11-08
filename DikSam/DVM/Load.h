#pragma once

#include "DikSamc.h"



class Load
{
public:
    Load();
    ~Load();

private:
    void ImplementDikSamFunction(DVM_VirtualMachine *pVirtualMachine, int iDestIndex, ExecutableEntry *pExecuatbleEntry, int iSrcIndex);
};

