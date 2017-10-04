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

class Native;
typedef DVM_Value DVM_NativeFunctionProc(Native *pThis, DVM_VirtualMachine *dvm, int arg_count, DVM_Value *args);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !DVM_DEV_H_INCLUDED
