#ifndef DVM_DEV_H_INCLUDED
#define DVM_DEV_H_INCLUDED

#include "DVM.h"

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#define DVM_DIKSAM_DEFAULT_PACKAGE_P1   "diksam"
#define DVM_DIKSAM_DEFAULT_PACKAGE_P2   "lang"
#define DVM_DIKSAM_DEFAULT_PACKAGE      (DVM_DIKSAM_DEFAULT_PACKAGE_P1 "." DVM_DIKSAM_DEFAULT_PACKAGE_P2)

#ifdef DKM_WINDOWS
#define FILE_SEPARATOR                  ('\\')
#define FILE_PATH_SEPARATOR             (";")
#else
#define FILE_SEPARATOR                  ('/')
#define FILE_PATH_SEPARATOR             (":")
#endif // DKM_WINDOWS

#define DIKSAM_REQUIRE_SUFFIX           (".dkh")
#define DIKSAM_IMPLEMENTATION_SUFFIX    (".dkm")

#define ARRAY_METHOD_SIZE       "size"
#define ARRAY_METHOD_RESIZE     "resize"
#define ARRAY_METHOD_INSERT     "insert"
#define ARRAY_METHOD_REMOVE     "remove"
#define ARRAY_METHOD_ADD        "add"
#define ARRAY_PREFIX            "array#"

#define STRING_METHOD_LENGTH    "length"
#define STRING_METHOD_SUBSTR    "substr"
#define STRING_PREFIX           "string#"

#define ARRAY_SIZE(arr)         (sizeof(arr) / sizeof((arr)[0]))

typedef enum
{
    SEARCH_FILE_SUCCESS,
    SEARCH_FILE_NOT_FOUND,
    SEARCH_FILE_PATH_TOO_LONG
} SearchFileStatus;

class Native;
typedef DVM_Value DVM_NativeFunctionProc(Native *pThis, DVM_VirtualMachine *dvm, int arg_count, DVM_Value *args);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !DVM_DEV_H_INCLUDED
