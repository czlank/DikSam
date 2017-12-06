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

    void CheckGC(DVM_VirtualMachine *pVirtualMachine);
    void GarbageCollect(DVM_VirtualMachine *pVirtualMachine);

    DVM_ObjectRef LiteralToString(DVM_VirtualMachine *pVirtualMachine, DVM_Char *str);
    DVM_ObjectRef CreateString(DVM_VirtualMachine *pVirtualMachine, DVM_Char *str);
    DVM_ObjectRef CreateArrayInt(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_ObjectRef CreateArrayDouble(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_ObjectRef CreateArrayObject(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_ObjectRef CreateClassObject(DVM_VirtualMachine *pVirtualMachine, int iIndex);

private:
    bool IsReferenceType(DVM_TypeSpecifier *pType);
    DVM_ObjectRef AllocObject(DVM_VirtualMachine *pVirtualMachine, ObjectType enType);
    DVM_ObjectRef AllocArray(DVM_VirtualMachine *pVirtualMachine, ArrayType enType, int iSize);
    void MarkObjects(DVM_VirtualMachine *pVirtualMachine);
    bool SweepObjects(DVM_VirtualMachine *pVirtualMachine);
    void ResetMark(DVM_Object *pObj);
    void Mark(DVM_ObjectRef *pRef);
    bool DisposeObject(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pObj);

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
    Util    &m_Util;
};