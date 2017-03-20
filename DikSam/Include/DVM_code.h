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
    DVM_BOOLEAN_TYPE,
    DVM_INT_TYPE,
    DVM_DOUBLE_TYPE,
    DVM_STRING_TYPE
} DVM_BasicType;

typedef wchar_t DVM_Char;

#ifdef __cplusplus
}
#endif // __cplusplus


#endif  /* PUBLIC_DVM_CODE_H_INCLUDED */