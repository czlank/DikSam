#pragma once

#include "DikSamc.h"

class OpcodeInfo
{
public:
    char    *mnemonic;
    char    *parameter;
    int     stack_increment;
};