#include "StdAfx.h"
#include "OpcodeInfo.h"

std::vector<OpcodeInfo> DVMOpcodeInfo::m_OpcodeInfo;

DVMOpcodeInfo::DVMOpcodeInfo()
{
    m_OpcodeInfo.emplace_back("dummy",                  "",    0);
    m_OpcodeInfo.emplace_back("push_int_1byte",         "b",   1);
    m_OpcodeInfo.emplace_back("push_int_2byte",         "s",   1);
    m_OpcodeInfo.emplace_back("push_int",               "p",   1);
    m_OpcodeInfo.emplace_back("push_double_0",          "",    1);
    m_OpcodeInfo.emplace_back("push_double_1",          "",    1);
    m_OpcodeInfo.emplace_back("push_double",            "p",   1);
    m_OpcodeInfo.emplace_back("push_string",            "p",   1);
    /**********/
    m_OpcodeInfo.emplace_back("push_stack_int",         "s",   1);
    m_OpcodeInfo.emplace_back("push_stack_double",      "s",   1);
    m_OpcodeInfo.emplace_back("push_stack_string",      "s",   1);
    m_OpcodeInfo.emplace_back("pop_stack_int",          "s",  -1);
    m_OpcodeInfo.emplace_back("pop_stack_double",       "s",  -1);
    m_OpcodeInfo.emplace_back("pop_stack_string",       "s",  -1);
    /**********/
    m_OpcodeInfo.emplace_back("push_static_int",        "s",   1);
    m_OpcodeInfo.emplace_back("push_static_double",     "s",   1);
    m_OpcodeInfo.emplace_back("push_static_string",     "s",   1);
    m_OpcodeInfo.emplace_back("pop_static_int",         "s",  -1);
    m_OpcodeInfo.emplace_back("pop_static_double",      "s",  -1);
    m_OpcodeInfo.emplace_back("pop_static_string",      "s",  -1);
    /**********/
    m_OpcodeInfo.emplace_back("add_int",                "",   -1);
    m_OpcodeInfo.emplace_back("add_double",             "",   -1);
    m_OpcodeInfo.emplace_back("add_string",             "",   -1);
    m_OpcodeInfo.emplace_back("sub_int",                "",   -1);
    m_OpcodeInfo.emplace_back("sub_double",             "",   -1);
    m_OpcodeInfo.emplace_back("mul_int",                "",   -1);
    m_OpcodeInfo.emplace_back("mul_double",             "",   -1);
    m_OpcodeInfo.emplace_back("div_int",                "",   -1);
    m_OpcodeInfo.emplace_back("div_double",             "",   -1);
    m_OpcodeInfo.emplace_back("mod_int",                "",   -1);
    m_OpcodeInfo.emplace_back("mod_double",             "",   -1);
    m_OpcodeInfo.emplace_back("minus_int",              "",    0);
    m_OpcodeInfo.emplace_back("minus_double",           "",    0);
    m_OpcodeInfo.emplace_back("increment",              "",    0);
    m_OpcodeInfo.emplace_back("decrement",              "",    0);
    m_OpcodeInfo.emplace_back("cast_int_to_double",     "",    0);
    m_OpcodeInfo.emplace_back("cast_double_to_int",     "",    0);
    m_OpcodeInfo.emplace_back("cast_boolean_to_string", "",    0);
    m_OpcodeInfo.emplace_back("cast_int_to_string",     "",    0);
    m_OpcodeInfo.emplace_back("cast_double_to_string",  "",    0);
    m_OpcodeInfo.emplace_back("eq_int",                 "",   -1);
    m_OpcodeInfo.emplace_back("eq_double",              "",   -1);
    m_OpcodeInfo.emplace_back("eq_string",              "",   -1);
    m_OpcodeInfo.emplace_back("gt_int",                 "",   -1);
    m_OpcodeInfo.emplace_back("gt_double",              "",   -1);
    m_OpcodeInfo.emplace_back("gt_string",              "",   -1);
    m_OpcodeInfo.emplace_back("ge_int",                 "",   -1);
    m_OpcodeInfo.emplace_back("ge_double",              "",   -1);
    m_OpcodeInfo.emplace_back("ge_string",              "",   -1);
    m_OpcodeInfo.emplace_back("lt_int",                 "",   -1);
    m_OpcodeInfo.emplace_back("lt_double",              "",   -1);
    m_OpcodeInfo.emplace_back("lt_string",              "",   -1);
    m_OpcodeInfo.emplace_back("le_int",                 "",   -1);
    m_OpcodeInfo.emplace_back("le_double",              "",   -1);
    m_OpcodeInfo.emplace_back("le_string",              "",   -1);
    m_OpcodeInfo.emplace_back("ne_int",                 "",   -1);
    m_OpcodeInfo.emplace_back("ne_double",              "",   -1);
    m_OpcodeInfo.emplace_back("ne_string",              "",   -1);
    m_OpcodeInfo.emplace_back("logical_and",            "",   -1);
    m_OpcodeInfo.emplace_back("logical_or",             "",   -1);
    m_OpcodeInfo.emplace_back("logical_not",            "",    0);
    m_OpcodeInfo.emplace_back("pop",                    "",   -1);
    m_OpcodeInfo.emplace_back("duplicate",              "",    1);
    m_OpcodeInfo.emplace_back("jump",                   "s",   0);
    m_OpcodeInfo.emplace_back("jump_if_true",           "s",  -1);
    m_OpcodeInfo.emplace_back("jump_if_false",          "s",  -1);
    /**********/
    m_OpcodeInfo.emplace_back("push_function",          "s",   0);
    m_OpcodeInfo.emplace_back("invoke",                 "",   -1);
    m_OpcodeInfo.emplace_back("return",                 "",   -1);
}

DVMOpcodeInfo::~DVMOpcodeInfo()
{

}

std::vector<OpcodeInfo>& DVMOpcodeInfo::Opcode()
{
    return m_OpcodeInfo;
}