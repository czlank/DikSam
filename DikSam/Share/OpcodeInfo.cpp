#include "StdAfx.h"
#include "OpcodeInfo.h"

std::map<int, OpcodeInfo> DVMOpcodeInfo::m_OpcodeInfo;

DVMOpcodeInfo::DVMOpcodeInfo()
{
    m_OpcodeInfo[0]                             = {"dummy",                  "",    0 };
    m_OpcodeInfo[DVM_PUSH_INT_1BYTE]            = {"push_int_1byte",         "b",   1};
    m_OpcodeInfo[DVM_PUSH_INT_2BYTE]            = {"push_int_2byte",         "s",   1};
    m_OpcodeInfo[DVM_PUSH_INT]                  = {"push_int",               "p",   1};
    m_OpcodeInfo[DVM_PUSH_DOUBLE_0]             = {"push_double_0",          "",    1};
    m_OpcodeInfo[DVM_PUSH_DOUBLE_1]             = {"push_double_1",          "",    1};
    m_OpcodeInfo[DVM_PUSH_DOUBLE]               = {"push_double",            "p",   1};
    m_OpcodeInfo[DVM_PUSH_STRING]               = {"push_string",            "p",   1};
    /**********/
    m_OpcodeInfo[DVM_PUSH_STACK_INT]            = {"push_stack_int",         "s",   1};
    m_OpcodeInfo[DVM_PUSH_STACK_DOUBLE]         = {"push_stack_double",      "s",   1};
    m_OpcodeInfo[DVM_PUSH_STACK_STRING]         = {"push_stack_string",      "s",   1};
    m_OpcodeInfo[DVM_POP_STACK_INT]             = {"pop_stack_int",          "s",  -1};
    m_OpcodeInfo[DVM_POP_STACK_DOUBLE]          = {"pop_stack_double",       "s",  -1};
    m_OpcodeInfo[DVM_POP_STACK_STRING]          = {"pop_stack_string",       "s",  -1};
    /**********/
    m_OpcodeInfo[DVM_PUSH_STATIC_INT]           = {"push_static_int",        "s",   1};
    m_OpcodeInfo[DVM_PUSH_STATIC_DOUBLE]        = {"push_static_double",     "s",   1};
    m_OpcodeInfo[DVM_PUSH_STATIC_STRING]        = {"push_static_string",     "s",   1};
    m_OpcodeInfo[DVM_POP_STATIC_INT]            = {"pop_static_int",         "s",  -1};
    m_OpcodeInfo[DVM_POP_STATIC_DOUBLE]         = {"pop_static_double",      "s",  -1};
    m_OpcodeInfo[DVM_POP_STATIC_STRING]         = {"pop_static_string",      "s",  -1};
    /**********/
    m_OpcodeInfo[DVM_ADD_INT]                   = {"add_int",                "",   -1};
    m_OpcodeInfo[DVM_ADD_DOUBLE]                = {"add_double",             "",   -1};
    m_OpcodeInfo[DVM_ADD_STRING]                = {"add_string",             "",   -1};
    m_OpcodeInfo[DVM_SUB_INT]                   = {"sub_int",                "",   -1};
    m_OpcodeInfo[DVM_SUB_DOUBLE]                = {"sub_double",             "",   -1};
    m_OpcodeInfo[DVM_MUL_INT]                   = {"mul_int",                "",   -1};
    m_OpcodeInfo[DVM_MUL_DOUBLE]                = {"mul_double",             "",   -1};
    m_OpcodeInfo[DVM_DIV_INT]                   = {"div_int",                "",   -1};
    m_OpcodeInfo[DVM_DIV_DOUBLE]                = {"div_double",             "",   -1};
    m_OpcodeInfo[DVM_MOD_INT]                   = {"mod_int",                "",   -1};
    m_OpcodeInfo[DVM_MOD_DOUBLE]                = {"mod_double",             "",   -1};
    m_OpcodeInfo[DVM_MINUS_INT]                 = {"minus_int",              "",    0};
    m_OpcodeInfo[DVM_MINUS_DOUBLE]              = {"minus_double",           "",    0};
    m_OpcodeInfo[DVM_INCREMENT]                 = {"increment",              "",    0};
    m_OpcodeInfo[DVM_DECREMENT]                 = {"decrement",              "",    0};
    m_OpcodeInfo[DVM_CAST_INT_TO_DOUBLE]        = {"cast_int_to_double",     "",    0};
    m_OpcodeInfo[DVM_CAST_DOUBLE_TO_INT]        = {"cast_double_to_int",     "",    0};
    m_OpcodeInfo[DVM_CAST_BOOLEAN_TO_STRING]    = {"cast_boolean_to_string", "",    0};
    m_OpcodeInfo[DVM_CAST_INT_TO_STRING]        = {"cast_int_to_string",     "",    0};
    m_OpcodeInfo[DVM_CAST_DOUBLE_TO_STRING]     = {"cast_double_to_string",  "",    0};
    m_OpcodeInfo[DVM_EQ_INT]                    = {"eq_int",                 "",   -1};
    m_OpcodeInfo[DVM_EQ_DOUBLE]                 = {"eq_double",              "",   -1};
    m_OpcodeInfo[DVM_EQ_STRING]                 = {"eq_string",              "",   -1};
    m_OpcodeInfo[DVM_GT_INT]                    = {"gt_int",                 "",   -1};
    m_OpcodeInfo[DVM_GT_DOUBLE]                 = {"gt_double",              "",   -1};
    m_OpcodeInfo[DVM_GT_STRING]                 = {"gt_string",              "",   -1};
    m_OpcodeInfo[DVM_GE_INT]                    = {"ge_int",                 "",   -1};
    m_OpcodeInfo[DVM_GE_DOUBLE]                 = {"ge_double",              "",   -1};
    m_OpcodeInfo[DVM_GE_STRING]                 = {"ge_string",              "",   -1};
    m_OpcodeInfo[DVM_LT_INT]                    = {"lt_int",                 "",   -1};
    m_OpcodeInfo[DVM_LT_DOUBLE]                 = {"lt_double",              "",   -1};
    m_OpcodeInfo[DVM_LT_STRING]                 = {"lt_string",              "",   -1};
    m_OpcodeInfo[DVM_LE_INT]                    = {"le_int",                 "",   -1};
    m_OpcodeInfo[DVM_LE_DOUBLE]                 = {"le_double",              "",   -1};
    m_OpcodeInfo[DVM_LE_STRING]                 = {"le_string",              "",   -1};
    m_OpcodeInfo[DVM_NE_INT]                    = {"ne_int",                 "",   -1};
    m_OpcodeInfo[DVM_NE_DOUBLE]                 = {"ne_double",              "",   -1};
    m_OpcodeInfo[DVM_NE_STRING]                 = {"ne_string",              "",   -1};
    m_OpcodeInfo[DVM_LOGICAL_AND]               = {"logical_and",            "",   -1};
    m_OpcodeInfo[DVM_LOGICAL_OR]                = {"logical_or",             "",   -1};
    m_OpcodeInfo[DVM_LOGICAL_NOT]               = {"logical_not",            "",    0};
    m_OpcodeInfo[DVM_POP]                       = {"pop",                    "",   -1};
    m_OpcodeInfo[DVM_DUPLICATE]                 = {"duplicate",              "",    1};
    m_OpcodeInfo[DVM_JUMP]                      = {"jump",                   "s",   0};
    m_OpcodeInfo[DVM_JUMP_IF_TRUE]              = {"jump_if_true",           "s",  -1};
    m_OpcodeInfo[DVM_JUMP_IF_FALSE]             = {"jump_if_false",          "s",  -1};
    /**********/
    m_OpcodeInfo[DVM_PUSH_FUNCTION]             = {"push_function",          "s",   0};
    m_OpcodeInfo[DVM_INVOKE]                    = {"invoke",                 "",   -1};
    m_OpcodeInfo[DVM_RETURN]                    = {"return",                 "",   -1};
}

DVMOpcodeInfo::~DVMOpcodeInfo()
{

}
