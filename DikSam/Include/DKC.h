#ifndef PUBLIC_DKC_H_INCLUDED
#define PUBLIC_DKC_H_INCLUDED

#include <stdio.h>
#include "DVM_code.h"

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

typedef struct DKC_Compiler_tag DKC_Compiler;

typedef enum
{
    DKC_FILE_INPUT_MODE = 1,
    DKC_STRING_INPUT_MODE
} DKC_InputMode;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  /* PUBLIC_DKC_H_INCLUDED */