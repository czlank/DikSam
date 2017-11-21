#pragma once

#include "DikSamc.h"
#include "GarbageCollect.h"

class Debug;
class Memory;
class Error;

class Execute
{
public:
    Execute(Debug& debug, Memory& memory, Error& error);
    ~Execute();

    DVM_Value operator () (DVM_Executable* pExecutable);

private:
    int ArrayGetInt(DVM_Object *pArray, int iIndex);
    double ArrayGetDouble(DVM_Object *pArray, int iIndex);
    DVM_Object* ArrayGetObject(DVM_Object *pArray, int iIndex);
    void ArraySetInt(DVM_Object* pArray, int iIndex, int value);
    void ArraySetDouble(DVM_Object* pArray, int iIndex, double value);
    void ArraySetObject(DVM_Object* pArray, int iIndex, DVM_Object *value);
    void CheckArray(DVM_Object *pArray, int iIndex, DVM_Executable *pExecutable, Function *pFunction, int iPC);
    void AddExecutable(DVM_Executable *pExecutable);
    void AddStaticVariables(DVM_Executable *pExecutable);
    DVM_Object* ChainString(DVM_Object *str1, DVM_Object *str2);
    void InitializeLocalVariables(DVM_Function *pFunction, int iFromSP);
    void ExpandStack(int iNeedStackSize);
    void InvokeNativeFunction(Function *pFunction, int *pSP);
    void InvokeDikSamFunction(Function **ppCaller, Function *pCallee, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pSP, int *pBase, DVM_Executable **ppExe);
    void ReturnFunction(Function **ppFunction, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pSP, int *pBase, DVM_Executable **ppExe);
    DVM_Object* CreateArraySub(int iDim, int iDimIndex, DVM_TypeSpecifier *pType);
    DVM_Object* CreateArray(int iDim, DVM_TypeSpecifier *pType);
    DVM_Object* CreateArrayLiteralInt(int iSize);
    DVM_Object* CreateArrayLiteralDouble(int iSize);
    DVM_Object* CreateArrayLiteralObject(int iSize);
    void RestorePC(DVM_Executable *pExecutable, Function *pFunction, int iPC);
    DVM_Value ExecuteCode(Function *pFunction, DVM_Byte *pCode, int iCodeSize);

    void DisposeVirtualMachine();

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
    Error   &m_Error;

    GarbageCollect      m_GarbageCollect;

    DVM_VirtualMachine  *m_pVirtualMachine;
};