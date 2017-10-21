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
    DVM_STRING_TYPE,
    DVM_CLASS_TYPE,
    DVM_NULL_TYPE,
    DVM_BASE_TYPE
} DVM_BasicType;

typedef struct DVM_TypeSpecifier_tag DVM_TypeSpecifier;

typedef struct 
{
    char                *name;
    DVM_TypeSpecifier   *type;
} DVM_LocalVariable;

typedef enum
{
    DVM_FUNCTION_DERIVE,
    DVM_ARRAY_DERIVE
} DVM_DeriveTag;

typedef struct 
{
    int                  parameter_count;
    DVM_LocalVariable   *parameter;
} DVM_FunctionDerive;

typedef struct 
{
    int     dummy;
} DVM_ArrayDerive;

typedef struct DVM_TypeDerive_tag
{
    DVM_DeriveTag   tag;
    union
    {
        DVM_FunctionDerive  function_d;
    } u;
} DVM_TypeDerive;

// 数据类型指示符
struct DVM_TypeSpecifier_tag
{
    DVM_BasicType        basic_type;    // 基础类型
    int                  class_index;
    int                  derive_count;  // 子类的数量
    DVM_TypeDerive      *derive;        // 子类
};

typedef wchar_t DVM_Char;
typedef unsigned char DVM_Byte;

typedef enum
{
    DVM_PUSH_INT_1BYTE = 1,
    DVM_PUSH_INT_2BYTE,
    DVM_PUSH_INT,
    DVM_PUSH_DOUBLE_0,
    DVM_PUSH_DOUBLE_1,
    DVM_PUSH_DOUBLE,
    DVM_PUSH_STRING,
    DVM_PUSH_NULL,
    /**********/
    DVM_PUSH_STACK_INT,
    DVM_PUSH_STACK_DOUBLE,
    DVM_PUSH_STACK_OBJECT,
    DVM_POP_STACK_INT,
    DVM_POP_STACK_DOUBLE,
    DVM_POP_STACK_OBJECT,
    /**********/
    DVM_PUSH_STATIC_INT,
    DVM_PUSH_STATIC_DOUBLE,
    DVM_PUSH_STATIC_OBJECT,
    DVM_POP_STATIC_INT,
    DVM_POP_STATIC_DOUBLE,
    DVM_POP_STATIC_OBJECT,
    /**********/
    DVM_PUSH_ARRAY_INT,
    DVM_PUSH_ARRAY_DOUBLE,
    DVM_PUSH_ARRAY_OBJECT,
    DVM_POP_ARRAY_INT,
    DVM_POP_ARRAY_DOUBLE,
    DVM_POP_ARRAY_OBJECT,
    /**********/
    DVM_PUSH_FIELD_INT,
    DVM_PUSH_FIELD_DOUBLE,
    DVM_PUSH_FIELD_OBJECT,
    DVM_POP_FIELD_INT,
    DVM_POP_FIELD_DOUBLE,
    DVM_POP_FIELD_OBJECT,
    /**********/
    DVM_ADD_INT,
    DVM_ADD_DOUBLE,
    DVM_ADD_STRING,
    DVM_SUB_INT,
    DVM_SUB_DOUBLE,
    DVM_MUL_INT,
    DVM_MUL_DOUBLE,
    DVM_DIV_INT,
    DVM_DIV_DOUBLE,
    DVM_MOD_INT,
    DVM_MOD_DOUBLE,
    DVM_MINUS_INT,
    DVM_MINUS_DOUBLE,
    DVM_INCREMENT,
    DVM_DECREMENT,
    DVM_CAST_INT_TO_DOUBLE,
    DVM_CAST_DOUBLE_TO_INT,
    DVM_CAST_BOOLEAN_TO_STRING,
    DVM_CAST_INT_TO_STRING,
    DVM_CAST_DOUBLE_TO_STRING,
    DVM_UP_CAST,
    DVM_DOWN_CAST,
    DVM_EQ_INT,
    DVM_EQ_DOUBLE,
    DVM_EQ_OBJECT,
    DVM_EQ_STRING,
    DVM_GT_INT,
    DVM_GT_DOUBLE,
    DVM_GT_STRING,
    DVM_GE_INT,
    DVM_GE_DOUBLE,
    DVM_GE_STRING,
    DVM_LT_INT,
    DVM_LT_DOUBLE,
    DVM_LT_STRING,
    DVM_LE_INT,
    DVM_LE_DOUBLE,
    DVM_LE_STRING,
    DVM_NE_INT,
    DVM_NE_DOUBLE,
    DVM_NE_OBJECT,
    DVM_NE_STRING,
    DVM_LOGICAL_AND,
    DVM_LOGICAL_OR,
    DVM_LOGICAL_NOT,
    DVM_POP,
    DVM_DUPLICATE,
    DVM_DUPLICATE_OFFSET,
    DVM_JUMP,
    DVM_JUMP_IF_TRUE,
    DVM_JUMP_IF_FALSE,
    /**********/
    DVM_PUSH_FUNCTION,
    DVM_PUSH_METHOD,
    DVM_INVOKE,
    DVM_RETURN,
    /**********/
    DVM_NEW,
    DVM_NEW_ARRAY,
    DVM_NEW_ARRAY_LITERAL_INT,
    DVM_NEW_ARRAY_LITERAL_DOUBLE,
    DVM_NEW_ARRAY_LITERAL_OBJECT,
    DVM_SUPER,
    DVM_INSTANCEOF
} DVM_Opcode;

typedef enum
{
    DVM_CONSTANT_INT,
    DVM_CONSTANT_DOUBLE,
    DVM_CONSTANT_STRING
} DVM_ConstantPoolTag;

// 常量池
typedef struct 
{
    DVM_ConstantPoolTag tag;    // 类型
    union                       // 实际的数据：整型、浮点型、字符串
    {
        int          c_int;
        double       c_double;
        DVM_Char    *c_string;
    } u;
} DVM_ConstantPool;

// 变量
typedef struct 
{
    char                *name;  // 变量名
    DVM_TypeSpecifier   *type;  // 类型
} DVM_Variable;

typedef struct 
{
    int line_number;
    int start_pc;
    int pc_count;
} DVM_LineNumber;

// 虚拟机的函数
typedef struct 
{
    DVM_TypeSpecifier   *type;                  // 类型
    char                *package_name;          // 所属的包名称
    char                *name;                  // 函数的名字
    int                  parameter_count;       // 形式参数个数
    DVM_LocalVariable   *parameter;             // 形式参数
    DVM_Boolean          is_implemented;        // 是否已经实现
    DVM_Boolean          is_method;             // 是否为类的方法
    int                  local_variable_count;  // 局部变量的数量
    DVM_LocalVariable   *local_variable;        // 局部变量
                                                // 对应的代码块
    int                  code_size;
    DVM_Byte            *code;
    int                  line_number_size;
    DVM_LineNumber      *line_number;
    int                  need_stack_size;
} DVM_Function;

// 虚拟机枚举
typedef struct
{
    char            *package_name;      // 所属的包名称
    char            *name;              // 枚举的名字
    DVM_Boolean      is_defined;        // 是否已经定义
    int              enumerator_count;  // 枚举内枚举值的数量
    char           **enumerator;        // 枚举值
} DVM_Enum;

// 虚拟机的常量
typedef struct
{
    DVM_TypeSpecifier   *type;          // 类型
    char                *package_name;  // 所属的包名称
    char                *name;          // 常量的名字
    DVM_Boolean          is_defined;    // 是否已经定义
} DVM_Constant;

typedef enum
{
    DVM_FILE_ACCESS,
    DVM_PUBLIC_ACCESS,
    DVM_PRIVATE_ACCESS
} DVM_AccessModifier;

typedef struct
{
    DVM_AccessModifier  access_modifier;
    char               *name;
    DVM_TypeSpecifier  *type;
} DVM_Field;

typedef struct
{
    DVM_AccessModifier  access_modifier;
    DVM_Boolean         is_abstract;
    DVM_Boolean         is_virtual;
    DVM_Boolean         is_override;
    char               *name;
} DVM_Method;

typedef enum
{
    DVM_CLASS_DEFINITION,
    DVM_INTERFACE_DEFINITION
} DVM_ClassOrInterface;

typedef struct
{
    char    *package_name;
    char    *name;
} DVM_ClassIdentifier;

// 虚拟机的类的定义
typedef struct
{
    DVM_Boolean             is_abstract;        // 是否为抽象类
    DVM_AccessModifier      access_modifier;    // 访问修饰符
    DVM_ClassOrInterface    class_or_interface; // 类还是接口
    char                   *package_name;       // 所属的包名称
    char                   *name;               // 类的名字
    DVM_Boolean             is_implemented;     // 是否实现了
    DVM_ClassIdentifier    *super_class;        // 父类
    int                     interface_count;    // 继承的接口的数量
    DVM_ClassIdentifier    *interface_;         // 继承的接口
    int                     field_count;        // 类内部变量（域）的数量
    DVM_Field              *field;              // 类内部变量
    int                     method_count;       // 方法的数量
    DVM_Method             *method;             // 方法
                                                // 构造函数 field_initializer
} DVM_Class;

struct DVM_Executable_tag
{
    char                *package_name;          // 包名称
    DVM_Boolean          is_required;           // 是否依赖别的包
    char                *path;                  // 路径
    int                  constant_pool_count;   // 常量池的数量
    DVM_ConstantPool    *constant_pool;         // 常量池
    int                  global_variable_count; // 全局变量的数量
    DVM_Variable        *global_variable;       // 全局变量
    int                  function_count;        // 函数的数量
    DVM_Function        *function;              // 函数
    int                  type_specifier_count;  // 类型说明符的数量
    DVM_TypeSpecifier   *type_specifier;        // 类型说明符
    int                  code_size;
    DVM_Byte            *code;
    int                  class_count;           // 类的数量
    DVM_Class           *class_definition;      // 类的定义
                                                // 枚举的数量 enum_count
                                                // 枚举的定义 enum_definition
                                                // 常量的数量 constant_count
                                                // 常量的定义 constant_definition
                                                // 顶层的代码块 top_level
                                                // 常量初始化的代码块--构造器 constant_initializer
    int                  line_number_size;
    DVM_LineNumber      *line_number;
    int                  need_stack_size;
};

typedef struct DVM_ExecutableItem_tag
{
    DVM_Executable                  *executable;
    struct DVM_ExecutableItem_tag   *next;
} DVM_ExecutableItem;

struct DVM_ExecutableList_tag
{
    DVM_Executable      *top_level;
    DVM_ExecutableItem  *list;
};

#ifdef __cplusplus
}
#endif // __cplusplus


#endif  /* PUBLIC_DVM_CODE_H_INCLUDED */