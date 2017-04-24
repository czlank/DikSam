#pragma once

#include "DikSamc.h"

typedef struct
{
    const char  *mnemonic;
    const char  *parameter;
    int         stack_increment;
} OpcodeInfo;

class DVMOpcodeInfo
{
public:
    DVMOpcodeInfo();
    ~DVMOpcodeInfo();

    static std::map<int, OpcodeInfo>& Opcode() { return m_OpcodeInfo; }

private:
    static std::map<int, OpcodeInfo> m_OpcodeInfo;
};