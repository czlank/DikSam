#ifndef PUBLIC_DVM_CODE_H_INCLUDED
#define PUBLIC_DVM_CODE_H_INCLUDED

#include <stdio.h>
#include <wchar.h>
#include "DVM.h"

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

typedef enum
{
    DVM_VOID_TYPE,
    DVM_BOOLEAN_TYPE,
    DVM_INT_TYPE,
    DVM_DOUBLE_TYPE,
    DVM_STRING_TYPE
} DVM_BasicType;

typedef struct DVM_TypeSpecifier_tag DVM_TypeSpecifier;

typedef struct 
{
    char                *name;
    DVM_TypeSpecifier   *type;
} DVM_LocalVariable;

typedef enum
{
    DVM_FUNCTION_DERIVE
} DVM_DeriveTag;

typedef struct 
{
    int                 parameter_count;
    DVM_LocalVariable   *parameter;
} DVM_FunctionDerive;

typedef struct DVM_TypeDerive_tag
{
    DVM_DeriveTag   tag;
    union
    {
        DVM_FunctionDerive  function_d;
    } u;
} DVM_TypeDerive;

struct DVM_TypeSpecifier_tag
{
    DVM_BasicType       basic_type;
    int                 derive_count;
    DVM_TypeDerive      *derive;
};

typedef wchar_t DVM_Char;
typedef unsigned char DVM_Byte;

typedef enum
{
    DVM_CONSTANT_INT,
    DVM_CONSTANT_DOUBLE,
    DVM_CONSTANT_STRING
} DVM_ConstantPoolTag;

typedef struct 
{
    DVM_ConstantPoolTag tag;
    union
    {
        int         c_int;
        double      c_double;
        DVM_Char    *c_string;
    } u;
} DVM_ConstantPool;

typedef struct 
{
    char                *name;
    DVM_TypeSpecifier   *type;
} DVM_Variable;

typedef struct 
{
    int line_number;
    int start_pc;
    int pc_count;
} DVM_LineNumber;

typedef struct 
{
    DVM_TypeSpecifier   *type;
    char                *name;
    int                 parameter_count;
    DVM_LocalVariable   *parameter;
    DVM_Boolean         is_implemented;
    int                 local_variable_count;
    DVM_LocalVariable   *local_variable;
    int                 code_size;
    DVM_Byte            *code;
    int                 line_number_size;
    DVM_LineNumber      *line_number;
    int                 need_stack_size;
} DVM_Function;

struct DVM_Executable_tag
{
    int                 constant_pool_count;
    DVM_ConstantPool    *constant_pool;
    int                 global_variable_count;
    DVM_Variable        *global_variable;
    int                 function_count;
    DVM_Function        *function;
    int                 code_size;
    DVM_Byte            *code;
    int                 line_number_size;
    DVM_LineNumber      *line_number;
    int                 need_stack_size;
};

#ifdef __cplusplus
}
#endif // __cplusplus


#endif  /* PUBLIC_DVM_CODE_H_INCLUDED */