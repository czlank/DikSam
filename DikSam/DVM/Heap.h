#pragma once

#include "DikSamc.h"
#include "DVM_pri.h"

class Debug;
class Memory;
class Util;

class CHeap
{
public:
    CHeap(Debug& debug, Memory& memory, Util& util);
    ~CHeap();

    void GarbageCollect(DVM_VirtualMachine *pVirtualMachine);

    DVM_ObjectRef LiteralToStringI(DVM_VirtualMachine *pVirtualMachine, DVM_Char *str);
    DVM_ObjectRef CreateString(DVM_VirtualMachine *pVirtualMachine, DVM_Char *str);
    DVM_ObjectRef CreateArrayIntI(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_Object* CreateArrayInt(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_ObjectRef CreateArrayDoubleI(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_Object* CreateArrayDouble(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_ObjectRef CreateArrayObjectI(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_Object* CreateArrayObject(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_ObjectRef CreateClassObjectI(DVM_VirtualMachine *pVirtualMachine, int iIndex);

private:
    void CheckGC(DVM_VirtualMachine *pVirtualMachine);
    DVM_ObjectRef AllocObject(DVM_VirtualMachine *pVirtualMachine, ObjectType enType);
    DVM_ObjectRef AllocArray(DVM_VirtualMachine *pVirtualMachine, ArrayType enType, int iSize);
    void MarkObjects(DVM_VirtualMachine *pVirtualMachine);
    void SweepObjects(DVM_VirtualMachine *pVirtualMachine);
    void ResetMark(DVM_Object *pObj);
    void Mark(DVM_Object *pObj);
    void DisposeObject(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pObj);

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
    Util    &m_Util;
};