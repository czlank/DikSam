#ifndef DVM_PRI_H_INCLUDED
#define DVM_PRI_H_INCLUDED

#include "DikSamc.h"
#include "DVM_dev.h"

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#define STACK_ALLOC_SIZE    (4096)
#define HEAP_THRESHOLD_SIZE (1024 * 256)

#define NULL_STRING         (TEXT("null"))
#define TRUE_STRING         (TEXT("true"))
#define FALSE_STRING        (TEXT("false"))

#define NO_LINE_NUMBER_PC   (-1)

typedef enum
{
    NATIVE_FUNCTION,
    DIKSAM_FUNCTION
} FunctionKind;

typedef struct 
{
    DVM_NativeFunctionProc  *proc;
    Native                  *pThis;
    int                      arg_count;
} NativeFunction;

typedef struct 
{
    DVM_Executable          *executable;
    int                     index;
} DiksamFunction;

// 在虚拟机中执行的函数
typedef struct
{
    //char            *package_name;  // 所属的包名称
    char            *name;          // 名字
    FunctionKind     kind;          // 类型
                                    // 是否已经实现 is_implemented
    union
    {
        NativeFunction  native_f;   // diksam内置函数
        DiksamFunction  diksam_f;   // 用户自定义函数
    } u;
} Function;

typedef struct
{
    Function    *caller;
    int          caller_address;
    int          base;
} CallInfo;

#define revalue_up_align(val)   ((val) ? (((val) - 1) / sizeof(DVM_Value) + 1) : 0)
#define CALL_INFO_ALIGN_SIZE    (revalue_up_align(sizeof(CallInfo)))

// 在虚拟机中执行代码用到的栈
typedef struct 
{
    int              alloc_size;        // 分配内存的大小
    int              stack_pointer;     // 栈顶
    DVM_Value       *stack;             // 栈实际的内存
    DVM_Boolean     *pointer_flags;     // 标志
} Stack;

typedef enum
{
    STRING_OBJECT = 1,
    ARRAY_OBJECT,
    OBJECT_TYPE_COUNT_PLUS_1
} ObjectType;

struct DVM_String_tag
{
    DVM_Boolean  is_literal;
    DVM_Char    *string;
};

typedef enum
{
    INT_ARRAY = 1,
    DOUBLE_ARRAY,
    OBJECT_ARRAY
} ArrayType;

struct DVM_Array_tag
{
    ArrayType        type;
    int              size;
    int              alloc_size;
    union
    {
        int         *int_array;
        double      *double_array;
        DVM_Object **object;
    } u;
};

struct DVM_Object_tag
{
    ObjectType      type;
    unsigned int    marked : 1;
    union
    {
        DVM_String  string;
        DVM_Array   array;
    } u;

    struct DVM_Object_tag   *prev;
    struct DVM_Object_tag   *next;
};

// 虚拟机执行代码用到的堆
typedef struct 
{
    int          current_heap_size;     // 当前堆的大小
    int          current_threshold;     // 阀值
    DVM_Object  *header;                // 堆的起始位置
} Heap;

typedef struct 
{
    int          variable_count;
    DVM_Value   *variable;
} Static;

struct DVM_VirtualMachine_tag
{
    Stack            stack;                 // 栈
    Heap             heap;                  // 堆
    Static           static_v;
    DVM_Executable  *current_executable;    // 当前的可执行对象
    Function        *current_function;      // 当前正在执行的函数
                                            // 当前的exception current_exception
    int              pc;                    // 程序计数器
    Function        *function;              // 函数
    int              function_count;        // 函数的数量
                                            // 类 classes
                                            // 类的数量 class_count
                                            // 枚举 enums
                                            // 枚举的数量 enum_count
                                            // 常量 constant
                                            // 常量的数量 constant_count
                                            // 可执行对象列表 executable_list
                                            // 可执行的入口 executable_entry
                                            // 顶层 top_level
                                            // 数组表 array_v_table
                                            // 字符串表 string_v_table
                                            // 当前的上下文 current_context
                                            // 已经释放的上下文 free_context
    DVM_Executable  *executable;
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  /* DVM_PRI_H_INCLUDED */