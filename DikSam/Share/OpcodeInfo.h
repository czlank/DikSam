#pragma once

#include "DikSamc.h"

class OpcodeInfo
{
public:
    OpcodeInfo(const char *lpstrmnemonic, const char *lpstrparameter, int stackincrement)
        : mnemonic(lpstrmnemonic), parameter(lpstrparameter), stack_increment(stackincrement)
    {}

    const char  *mnemonic;
    const char  *parameter;
    int         stack_increment;
};

class DVMOpcodeInfo
{
public:
    DVMOpcodeInfo();
    ~DVMOpcodeInfo();

    static std::vector<OpcodeInfo>& Opcode();

private:
    static std::vector<OpcodeInfo> m_OpcodeInfo;
};