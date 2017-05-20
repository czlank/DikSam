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

#define TRUE_STRING (L"true")
#define FALSE_STRING (L"false")

#define NO_LINE_NUMBER_PC   (-1)

typedef enum
{
    NATIVE_FUNCTION,
    DIKSAM_FUNCTION
} FunctionKind;

class Native;

typedef struct 
{
    DVM_NativeFunctionProc  *proc;
    Native                  *pThis;
    int                     arg_count;
} NativeFunction;

typedef struct 
{
    DVM_Executable          *executable;
    int                     index;
} DiksamFunction;

typedef struct 
{
    char            *name;
    FunctionKind    kind;
    union
    {
        NativeFunction  native_f;
        DiksamFunction  diksam_f;
    } u;
} Function;

typedef struct
{
    Function    *caller;
    int         caller_address;
    int         base;
} CallInfo;

#define revalue_up_align(val)   ((val) ? (((val) - 1) / sizeof(DVM_Value) + 1) : 0)
#define CALL_INFO_ALIGN_SIZE    (revalue_up_align(sizeof(CallInfo)))

typedef struct 
{
    int             alloc_size;
    int             stack_pointer;
    DVM_Value       *stack;
    DVM_Boolean     *pointer_flags;
} Stack;

typedef enum
{
    STRING_OBJECT = 1,
    OBJECT_TYPE_COUNT_PLUS_1
} ObjectType;

struct DVM_String_tag
{
    DVM_Boolean is_literal;
    DVM_Char    *string;
};

struct DVM_Object_tag
{
    ObjectType      type;
    unsigned int    marked : 1;
    union
    {
        DVM_String  string;
    } u;

    struct DVM_Object_tag   *prev;
    struct DVM_Object_tag   *next;
};

typedef struct 
{
    int         current_heap_size;
    int         current_threshold;
    DVM_Object  *header;
} Heap;

typedef struct 
{
    int         variable_count;
    DVM_Value   *variable;
} Static;

struct DVM_VirtualMachine_tag
{
    Stack           stack;
    Heap            heap;
    Static          static_v;
    int             pc;
    Function        *function;
    int             function_count;
    DVM_Executable  *executable;
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  /* DVM_PRI_H_INCLUDED */