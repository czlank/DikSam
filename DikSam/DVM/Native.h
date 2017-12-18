#pragma once

#include "DikSamc.h"
#include "DVM_dev.h"

class Debug;
class Memory;
class Error;
class CHeap;
class Load;

class Native
{
public:
    Native(Debug& debug, Memory& memory, Error& error, CHeap& heap, Load& load);
    ~Native();

    void AddNativeFunctions(DVM_VirtualMachine *pVirtualMachine);

private:
    static DVM_Value PrintProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    static DVM_Value SleepProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);

    static DVM_Value ArraySizeProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    static DVM_Value ArrayResizeProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    static DVM_Value ArrayInsertProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    static DVM_Value ArrayRemoveProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    static DVM_Value ArrayAddProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);

    static DVM_Value StringLengthProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    static DVM_Value StringSubStrProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);

    DVM_Value Print(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    DVM_Value Sleep(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);

    DVM_Value ArraySizeProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    DVM_Value ArrayResizeProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    DVM_Value ArrayInsertProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    DVM_Value ArrayRemoveProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    DVM_Value ArrayAddProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);

    DVM_Value StringLengthProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    DVM_Value StringSubStrProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);

    int ArraySize(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pArray);
    void ArrayResize(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pArray, int iNewSize);
    void ArrayInsert(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pArray, int iPos, DVM_Value value);
    void ArrayRemove(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pArray, int iPos);

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
    Error   &m_Error;
    CHeap   &m_Heap;
    Load    &m_Load;
};