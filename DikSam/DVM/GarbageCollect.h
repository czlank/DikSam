#pragma once

#include "DikSamc.h"

class Debug;
class Memory;

class GarbageCollect
{
#define GARBAGECOLLECT_MEM_Malloc(size)             (m_Memory.Malloc(__FILE__, __LINE__, size))
#define GARBAGECOLLECT_MEM_Free(ptr)                (m_Memory.Free(ptr))
#define GARBAGECOLLECT_DBG_Assert(expression, arg)  ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

public:
    GarbageCollect(Debug& debug, Memory& memory);
    ~GarbageCollect();

    void GC(DVM_VirtualMachine *pVirtualMachine);
    DVM_Object* LiteralToString(DVM_VirtualMachine *pVirtualMachine, DVM_Char *str);
    DVM_Object* CreateString(DVM_VirtualMachine *pVirtualMachine, DVM_Char *str);

private:
    void CheckGC(DVM_VirtualMachine *pVirtualMachine);
    DVM_Object* AllocObject(DVM_VirtualMachine *pVirtualMachine, ObjectType enType);
    void MarkObjects(DVM_VirtualMachine *pVirtualMachine);
    void SweepObjects(DVM_VirtualMachine *pVirtualMachine);
    void ResetMark(DVM_Object *pObj);
    void Mark(DVM_Object *pObj);
    void DisposeObject(DVM_VirtualMachine *pVirtualMachine, DVM_Object *pObj);

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
};