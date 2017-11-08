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
#define ARRAY_ALLOC_SIZE    (256)

#define NULL_STRING                     (TEXT("null"))
#define TRUE_STRING                     (TEXT("true"))
#define FALSE_STRING                    (TEXT("false"))
#define BUILD_IN_METHOD_PACKAGE_NAME    ("$build_in")

#define NO_LINE_NUMBER_PC   (-1)
#define FUNCTION_NOT_FOUND  (-1)
#define FIELD_NOT_FOUND     (-1)
#define CALL_FROM_NATIVE    (-1)

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
    DVM_Boolean              is_method;
    DVM_Boolean              return_pointer;
} NativeFunction;

typedef struct ExecutableEntry_tag ExecutableEntry;

typedef struct 
{
    ExecutableEntry        *executable;
    int                     index;
} DiksamFunction;

// ���������ִ�еĺ���
typedef struct
{
    char            *package_name;  // �����İ�����
    char            *name;          // ����
    FunctionKind     kind;          // ����
    DVM_Boolean      is_implemented;// �Ƿ��Ѿ�ʵ��
    union
    {
        NativeFunction  native_f;   // diksam���ú���
        DiksamFunction  diksam_f;   // �û��Զ��庯��
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

// ���������ִ�д����õ���ջ
typedef struct 
{
    int              alloc_size;        // �����ڴ�Ĵ�С
    int              stack_pointer;     // ջ��
    DVM_Value       *stack;             // ջʵ�ʵ��ڴ�
    DVM_Boolean     *pointer_flags;     // ��־
} Stack;

typedef enum
{
    STRING_OBJECT = 1,
    ARRAY_OBJECT,
    CLASS_OBJECT,
    OBJECT_TYPE_COUNT_PLUS_1
} ObjectType;

struct DVM_String_tag
{
    DVM_Boolean  is_literal;
    int          length;
    DVM_Char    *string;
};

typedef enum
{
    INT_ARRAY = 1,
    DOUBLE_ARRAY,
    OBJECT_ARRAY,
    FUNCTION_INDEX_ARRAY
} ArrayType;

struct DVM_Array_tag
{
    ArrayType        type;
    int              size;
    int              alloc_size;
    union
    {
        int             *int_array;
        double          *double_array;
        DVM_ObjectRef   *object;
        int             *function_index;
    } u;
};

typedef struct
{
    int         field_count;
    DVM_Value  *field;
} DVM_ClassObject;

struct DVM_Object_tag
{
    ObjectType      type;
    unsigned int    marked : 1;
    union
    {
        DVM_String          string;
        DVM_Array           array;
        DVM_ClassObject     class_object;
    } u;

    struct DVM_Object_tag   *prev;
    struct DVM_Object_tag   *next;
};

// �����ִ�д����õ��Ķ�
typedef struct 
{
    int          current_heap_size;     // ��ǰ�ѵĴ�С
    int          current_threshold;     // ��ֵ
    DVM_Object  *header;                // �ѵ���ʼλ��
} Heap;

typedef struct 
{
    int          variable_count;
    DVM_Value   *variable;
} Static;

typedef struct ExecClass_tag
{
    DVM_Class               *dvm_class;
    ExecutableEntry         *executable;
    char                    *package_name;
    char                    *name;
    DVM_Boolean              is_implemented;
    int                      class_index;
    struct ExecClass_tag    *super_class;
    DVM_VTable              *class_table;
    int                      interface_count;
    struct ExecClass_tag   **interface_;
    DVM_VTable             **interface_v_table;
    int                      field_count;
    DVM_TypeSpecifier      **field_type;
} ExecClass;

typedef struct
{
    char        *name;
    int          index;
} VTableItem;

struct DVM_VTable_tag
{
    ExecClass   *exec_class;
    int          table_size;
    VTableItem  *table;
};

struct ExecutableEntry_tag
{
    DVM_Executable             *executable;
    Static                      static_v;
    struct ExecutableEntry_tag *next;
};

typedef struct
{
    char            *package_name;
    char            *name;
    DVM_Boolean      is_defined;
} Enum;

typedef struct
{
    char            *package_name;
    char            *name;
    DVM_Boolean      is_defined;
    DVM_Value        value;
} Constant;

struct DVM_VirtualMachine_tag
{
    Stack                stack;                 // ջ
    Heap                 heap;                  // ��
    ExecutableEntry     *current_executable;    // ��ǰ�Ŀ�ִ�ж���
    Function            *current_function;      // ��ǰ����ִ�еĺ���
    DVM_ObjectRef        current_exception;     // ��ǰ��exception
    int                  pc;                    // ���������
    Function           **function;              // ����
    int                  function_count;        // ����������
    ExecClass          **classes                // ��
    int                  class_count;           // �������
                                            // ö�� enums
                                            // ö�ٵ����� enum_count
                                            // ���� constant
                                            // ���������� constant_count
    DVM_ExecutableList  *executable_list;       // ��ִ�ж����б�
    ExecutableEntry     *executable_entry;      // ��ִ�е����
    ExecutableEntry     *top_level;             // ����
    DVM_VTable          *array_v_table;         // �����
    DVM_VTable          *string_v_table;        // �ַ�����
                                            // ��ǰ�������� current_context
                                            // �Ѿ��ͷŵ������� free_context
};

typedef struct RefInNativeFunc_tag
{
    DVM_Object_tag              object;
    struct RefInNativeFunc_tag *next;
} RefInNativeFunc;

struct DVM_Context_tag
{
    RefInNativeFunc            *ref_in_native_method;
    struct DVM_Context_tag     *next;
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  /* DVM_PRI_H_INCLUDED */