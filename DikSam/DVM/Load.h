#pragma once

#include "DikSamc.h"
#include "DVM_pri.h"
#include "Native.h"

class Debug;
class Memory;
class Util;
class Error;
class Interface;

class Load
{
public:
    Load(Debug& debug, Memory& memory, Util& util, Error& error, Interface& interfaceRef);
    ~Load();

    DVM_VirtualMachine* CreateVirtualMachine();
    void SetExecutable(DVM_VirtualMachine *pVirtualMachine, DVM_ExecutableList *pList);
    void AddNativeFunction(DVM_VirtualMachine *pVirtualMachine, char *lpstrPackageName, char *lpstrFuncName, DVM_NativeFunctionProc *pProc, int iArgCount, DVM_Boolean bIsMethod, DVM_Boolean bReturnPointer);
    void DynamicLoad(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pCallerExecutable, Function *pCaller, int iPC, Function *pFunc);

private:
    void ImplementDikSamFunction(DVM_VirtualMachine *pVirtualMachine, int iDestIndex, ExecutableEntry *pExecuatbleEntry, int iSrcIndex);
    void AddFunctions(DVM_VirtualMachine *pVirtualMachine, ExecutableEntry *pExecutableEntry);
    int SearchFunction(DVM_VirtualMachine *pVirtualMachine, char *lpstrPackageName, char *lpstrName);
    int SearchClass(DVM_VirtualMachine *pVirtualMachine, char *lpstrPackageName, char *lpstrName);
    void ConvertCode(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, DVM_Byte *pCode, int iCodeSize, DVM_Function *pFunction);
    void AddStaticVariables(ExecutableEntry *pExecutableEntry, DVM_Executable *pExecutable);
    DVM_Class* SearchClassFromExecutable(DVM_Executable *pExecutable, char *lpstrName);
    int SetFieldTypes(DVM_Executable *pExecutable, DVM_Class *pPos, DVM_TypeSpecifier **ppFieldType, int iIndex);
    void AddFields(DVM_Executable *pExecutable, DVM_Class *pSrc, ExecClass *pDest);
    void SetVTable(DVM_VirtualMachine *pVirtualMachine, DVM_Class *pClass, DVM_Method *pSrc, VTableItem *pDest, bool bSetName);
    int AddMethod(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, DVM_Class *pPos, DVM_VTable *pVTable);
    DVM_VTable* AllocVTable(ExecClass *pExecClass);
    void AddMethods(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, DVM_Class *pSrc, ExecClass *pDest);
    void AddClass(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, DVM_Class *pSrc, ExecClass *pDest);
    void SetSuperClass(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, int iOldClassCount);
    void AddClasses(DVM_VirtualMachine *pVirtualMachine, ExecutableEntry *pExecutableEntry);
    ExecutableEntry* AddExecutableToDvm(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, DVM_Boolean bIsTopLevel);
    void SetBuiltInMethods(DVM_VirtualMachine *pVirtualMachine);

    inline int Get2ByteInt(DVM_Byte *p) { return ((p[0] << 8) + p[1]); }
    inline void Set2ByteInt(DVM_Byte *p, int value) { p[0] = value >> 8, p[1] = value & 0xFF; }

private:
    Debug       &m_Debug;
    Memory      &m_Memory;
    Util        &m_Util;
    Error       &m_Error;
    Interface   &m_Interface;

    Native   m_Native;

    static VTableItem       m_stArrayMethodVTable[];
    static VTableItem       m_stStringMethodVTable[];
};
