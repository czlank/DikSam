#ifndef PUBLIC_DVM_H_INCLUDED
#define PUBLIC_DVM_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

typedef struct DVM_Executable_tag       DVM_Executable;
typedef struct DVM_ExecutableList_tag   DVM_ExecutableList;
typedef struct DVM_VirtualMachine_tag   DVM_VirtualMachine;

typedef struct DVM_Object_tag           DVM_Object;
typedef struct DVM_String_tag           DVM_String;
typedef struct DVM_Array_tag            DVM_Array;

typedef enum
{
    DVM_FALSE = 0,
    DVM_TRUE = 1
} DVM_Boolean;

typedef union
{
    int          int_value;
    double       double_value;
    DVM_Object  *object;
} DVM_Value;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  /* PUBLIC_DVM_CODE_H_INCLUDED */