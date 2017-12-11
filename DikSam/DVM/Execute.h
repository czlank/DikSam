#pragma once

#include "DikSamc.h"
#include "Heap.h"

class Debug;
class Memory;
class Util;
class Error;
class Interface;

class Execute
{
public:
    Execute(Debug& debug, Memory& memory, Util& uitl, Error& error, Interface& interfaceRef);
    ~Execute();

    DVM_Value operator () (DVM_VirtualMachine *pVirtualMachine);
    void DisposeExecutableList(DVM_ExecutableList *pList);
    void DisposeVirtualMachine(DVM_VirtualMachine *pVirtualMachine);

private:
    int ArrayGetInt(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef array, int iIndex);
    double ArrayGetDouble(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef array, int iIndex);
    DVM_ObjectRef ArrayGetObject(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef array, int iIndex);
    void ArraySetInt(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef array, int iIndex, int value);
    void ArraySetDouble(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef array, int iIndex, double value);
    void ArraySetObject(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef array, int iIndex, DVM_ObjectRef value);
    void CheckArray(DVM_ObjectRef array, int iIndex, DVM_Executable *pExecutable, Function *pFunction, int iPC);
    DVM_ObjectRef ChainString(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef str1, DVM_ObjectRef str2);
    void InitializeLocalVariables(DVM_VirtualMachine *pVirtualMachine, DVM_Function *pFunction, int iFromSP);
    void ExpandStack(DVM_VirtualMachine *pVirtualMachine, int iNeedStackSize);
    void InvokeNativeFunction(DVM_VirtualMachine *pVirtualMachine, Function *pCaller, Function *pCallee, int iPC, int *pSP, int iBase);
    void InvokeDikSamFunction(DVM_VirtualMachine *pVirtualMachine, Function **ppCaller, Function *pCallee, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pSP, int *pBase, ExecutableEntry **ppExeEntry, DVM_Executable **ppExe);
    DVM_Boolean DoReturn(DVM_VirtualMachine *pVirtualMachine, Function **ppFunction, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pBase, ExecutableEntry **ppExeEntry, DVM_Executable **ppExe);
    DVM_Boolean ReturnFunction(DVM_VirtualMachine *pVirtualMachine, Function **ppFunction, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pBase, ExecutableEntry **ppExeEntry, DVM_Executable **ppExe);
    DVM_ObjectRef CreateArraySub(DVM_VirtualMachine *pVirtualMachine, int iDim, int iDimIndex, DVM_TypeSpecifier *pType);
    DVM_ObjectRef CreateArray(DVM_VirtualMachine *pVirtualMachine, int iDim, DVM_TypeSpecifier *pType);
    DVM_ObjectRef CreateArrayLiteralInt(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_ObjectRef CreateArrayLiteralDouble(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_ObjectRef CreateArrayLiteralObject(DVM_VirtualMachine *pVirtualMachine, int iSize);
    void RestorePC(DVM_VirtualMachine *pVirtualMachine, ExecutableEntry *pExecutableEntry, Function *pFunction, int iPC);
    DVM_Value ExecuteCode(DVM_VirtualMachine *pVirtualMachine, Function *pFunction, DVM_Byte *pCode, int iCodeSize, int iBase);
    void CheckNullPointer(DVM_Executable *pExecutable, Function *pFunction, int iPC, DVM_ObjectRef *pObj);
    DVM_Boolean CheckInstanceOf(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef *pObj, int iTargetIdx, DVM_Boolean *pIsInterface = nullptr, int *pInterfaceIdx = nullptr);
    void CheckDownCast(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, Function *pFunction, int iPC, DVM_ObjectRef *pObject, int iTargetIdx, DVM_Boolean *pIsSameClass, DVM_Boolean *pIsInterface, int *pInterfaceIndex);
    void PushObject(DVM_VirtualMachine *pVirtualMachine, DVM_Value value);
    DVM_Value PopObject(DVM_VirtualMachine *pVirtualMachine);
    void DisposeVTable(DVM_VTable *pVTable);
    int GetFieldIndexSub(ExecClass *pExecClass, char *lpstrFieldName, int *pSuperCount);
    int GetFieldIndex(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef obj, char *lpstrFieldName);

    inline bool IsPointerType(DVM_TypeSpecifier *pType);

private:
    Debug       &m_Debug;
    Memory      &m_Memory;
    Util        &m_Util;
    Error       &m_Error;
    Interface   &m_Interface;

    CHeap       m_Heap;
};