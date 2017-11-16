#include "stdafx.h"
#include "Debug.h"
#include "Memory.h"
#include "Util.h"
#include "Error.h"
#include "OpcodeInfo.h"
#include "Load.h"

#ifdef DBG_assert
#undef DBG_assert
#endif
#define DBG_assert(expression, arg) ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

#ifdef DBG_panic
#undef DBG_panic
#endif
#define DBG_panic(arg)                      (m_Debug.Panic(__FILE__, __LINE__, arg))

#ifdef MEM_malloc
#undef MEM_malloc
#endif
#define MEM_malloc(size)                    (m_Memory.Malloc(__FILE__, __LINE__, size))

#ifdef MEM_realloc
#undef MEM_realloc
#endif
#define MEM_realloc(ptr, size)              (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))

#ifdef MEM_free
#undef MEM_free
#endif
#define MEM_free(ptr)                       (m_Memory.Free(ptr))

#ifdef MEM_strdup
#undef MEM_strdup
#endif
#define MEM_strdup(ptr)                     (m_Memory.StrDUP(__FILE__, __LINE__, ptr))

Load::Load(Debug& debug, Memory& memory, Util& util, Error& error)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Util(util)
    , m_Error(error)
    
{
}

Load::~Load()
{
}

void Load::ImplementDikSamFunction(DVM_VirtualMachine *pVirtualMachine, int iDestIndex, ExecutableEntry *pExecuatbleEntry, int iSrcIndex)
{
    pVirtualMachine->function[iDestIndex]->u.diksam_f.executable = pExecuatbleEntry;
    pVirtualMachine->function[iDestIndex]->u.diksam_f.index = iSrcIndex;
}

void Load::AddFunctions(DVM_VirtualMachine *pVirtualMachine, ExecutableEntry *pExecutableEntry)
{
    int iAddFuncCount = 0;
    DVM_Boolean *pNewFuncFlags = (DVM_Boolean*)MEM_malloc(sizeof(DVM_Boolean) * pExecutableEntry->executable->function_count);

    for (int iSrcIndex = 0; iSrcIndex < pExecutableEntry->executable->function_count; iSrcIndex++)
    {
        int iDestIndex = 0;

        for (; iDestIndex < pVirtualMachine->function_count; iDestIndex++)
        {
            if (std::string(pVirtualMachine->function[iDestIndex]->name) == pExecutableEntry->executable->function[iSrcIndex].name
                && m_Util.ComparePackageName(pVirtualMachine->function[iDestIndex]->package_name, pExecutableEntry->executable->function[iSrcIndex].package_name))
            {
                if (DVM_TRUE == pExecutableEntry->executable->function[iSrcIndex].is_implemented)
                {
                    char *lpstrPackageName = "";

                    if (pVirtualMachine->function[iDestIndex]->package_name)
                    {
                        lpstrPackageName = pVirtualMachine->function[iDestIndex]->package_name;
                    }

                    m_Error.CompileError(NO_LINE_NUMBER_PC, FUNCTION_MULTIPLE_DEFINE_ERR,
                        STRING_MESSAGE_ARGUMENT, "package", lpstrPackageName, STRING_MESSAGE_ARGUMENT, "name", pVirtualMachine->function[iDestIndex]->name,
                        MESSAGE_ARGUMENT_END);
                }

                pNewFuncFlags[iSrcIndex] = DVM_FALSE;

                if (DVM_TRUE == pExecutableEntry->executable->function[iSrcIndex].is_implemented)
                {
                    ImplementDikSamFunction(pVirtualMachine, iDestIndex, pExecutableEntry, iSrcIndex);
                }

                break;
            }
        }

        if (iDestIndex == pVirtualMachine->function_count)
        {
            pNewFuncFlags[iSrcIndex] = DVM_TRUE;
            iAddFuncCount++;
        }
    }

    pVirtualMachine->function = (Function**)MEM_realloc(pVirtualMachine->function, sizeof(Function*) * (pVirtualMachine->function_count + iAddFuncCount));

    for (int iSrcIndex = 0, iDestIndex = pVirtualMachine->function_count; iSrcIndex < pExecutableEntry->executable->function_count; iSrcIndex++)
    {
        if (pNewFuncFlags[iSrcIndex] != DVM_TRUE)
            continue;

        pVirtualMachine->function[iDestIndex] = (Function*)MEM_malloc(sizeof(Function));

        if (pExecutableEntry->executable->function[iSrcIndex].package_name)
        {
            pVirtualMachine->function[iDestIndex]->package_name = MEM_strdup(pExecutableEntry->executable->function[iSrcIndex].package_name);
        }
        else
        {
            pVirtualMachine->function[iDestIndex]->package_name = nullptr;
        }

        pVirtualMachine->function[iDestIndex]->name = MEM_strdup(pExecutableEntry->executable->function[iSrcIndex].name);
        pVirtualMachine->function[iDestIndex]->kind = DIKSAM_FUNCTION;
        pVirtualMachine->function[iDestIndex]->is_implemented = pExecutableEntry->executable->function[iSrcIndex].is_implemented;

        if (DVM_TRUE == pExecutableEntry->executable->function[iSrcIndex].is_implemented)
        {
            ImplementDikSamFunction(pVirtualMachine, iDestIndex, pExecutableEntry, iSrcIndex);
        }

        iDestIndex++;
    }

    pVirtualMachine->function_count += iAddFuncCount;

    MEM_free(pNewFuncFlags);
}

int Load::SearchFunction(DVM_VirtualMachine *pVirtualMachine, char *lpstrPackageName, char *lpstrName)
{
    for (int i = 0; i < pVirtualMachine->function_count; i++)
    {
        if (m_Util.ComparePackageName(pVirtualMachine->function[i]->package_name, lpstrPackageName)
            && std::string(pVirtualMachine->function[i]->name) == lpstrName)
        {
            return i;
        }
    }

    return FUNCTION_NOT_FOUND;
}

int Load::SearchClass(DVM_VirtualMachine *pVirtualMachine, char *lpstrPackageName, char *lpstrName)
{
    for (int i = 0; i < pVirtualMachine->class_count; i++)
    {
        if (m_Util.ComparePackageName(pVirtualMachine->classes[i]->package_name, lpstrPackageName)
            && std::string(pVirtualMachine->classes[i]->name, lpstrName))
        {
            return i;
        }
    }

    m_Error.CompileError(NO_LINE_NUMBER_PC, CLASS_NOT_FOUND_ERR,
        STRING_MESSAGE_ARGUMENT, "name", lpstrName,
        MESSAGE_ARGUMENT_END);

    return 0;
}

void Load::ConvertCode(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, DVM_Byte *pCode, int iCodeSize, DVM_Function *pFunction)
{
    for (int i = 0; i < iCodeSize; i++)
    {
        switch (pCode[i])
        {
        case DVM_PUSH_STACK_INT :
        case DVM_PUSH_STACK_DOUBLE :
        case DVM_PUSH_STACK_OBJECT :
        case DVM_POP_STACK_INT :
        case DVM_POP_STACK_DOUBLE :
        case DVM_POP_STACK_OBJECT:
            {
                int iParameterCount;
            
                DBG_assert(pFunction, ("pFunction == nullptr"));

                if (DVM_TRUE == pFunction->is_method)
                {
                    iParameterCount = pFunction->parameter_count + 1;   // for this
                }
                else
                {
                    iParameterCount = pFunction->parameter_count;
                }

                int iSrcIndex = Get2ByteInt(&pCode[i + 1]);
                int iDestIndex = 0;
                if (iSrcIndex >= iParameterCount)
                {
                    iDestIndex = iSrcIndex + CALL_INFO_ALIGN_SIZE;
                }
                else
                {
                    iDestIndex = iSrcIndex;
                }

                Set2ByteInt(&pCode[i + 1], iDestIndex);
            }
            break;

        case DVM_PUSH_FUNCTION:
            {
                int iIndexInExe = Get2ByteInt(&pCode[i + 1]);
                unsigned int iFuncIndex = SearchFunction(pVirtualMachine, pExecutable->function[iIndexInExe].package_name, pExecutable->function[iIndexInExe].name);
                Set2ByteInt(&pCode[i + 1], iFuncIndex);
            }
            break;

        case DVM_NEW :
        case DVM_INSTANCEOF :
        case DVM_DOWN_CAST:
            {
                int iIndexInExe = Get2ByteInt(&pCode[i + 1]);
                unsigned int iClassIndex = SearchClass(pVirtualMachine, pExecutable->class_definition[iIndexInExe].package_name, pExecutable->class_definition[iIndexInExe].name);
                Set2ByteInt(&pCode[i + 1], iClassIndex);
            }
            break;
        }

        OpcodeInfo info = DVMOpcodeInfo::Opcode()[pCode[i]];

        for (int j = 0; info.parameter[j] != '\0'; j++)
        {
            switch (info.parameter[j])
            {
            case 'b' :
                i++;
                break;

            case 's' :
            case 'p' :
                i += 2;
                break;

            default :
                DBG_assert(0, ("param..", info.parameter, ", index..", j));
            }
        }
    }
}

void Load::AddStaticVariables(ExecutableEntry *pExecutableEntry, DVM_Executable *pExecutable)
{
    pExecutableEntry->static_v.variable = (DVM_Value*)MEM_malloc(sizeof(DVM_Value) * pExecutable->global_variable_count);
    pExecutableEntry->static_v.variable_count = pExecutable->global_variable_count;

    for (int i = 0; i < pExecutable->global_variable_count; i++)
    {
        if (DVM_STRING_TYPE == pExecutable->global_variable[i].type->basic_type)
        {
            pExecutableEntry->static_v.variable[i].object = {nullptr, nullptr};
        }
    }

    for (int i = 0; i < pExecutable->global_variable_count; i++)
    {
        m_Util.InitializeValue(pExecutable->global_variable[i].type, &pExecutableEntry->static_v.variable[i]);
    }
}

DVM_Class* Load::SearchClassFromExecutable(DVM_Executable *pExecutable, char *lpstrName)
{
    for (int i = 0; i < pExecutable->class_count; i++)
    {
        if (std::string(pExecutable->class_definition[i].name) == lpstrName)
        {
            return &pExecutable->class_definition[i];
        }
    }

    DBG_panic(("class ", lpstrName, " not found."));

    return nullptr;
}

int SetFieldTypes(DVM_Executable *pExecutable, DVM_Class *pPos, DVM_TypeSpecifier **ppFieldType, int iIndex)
{
    if (pPos->super_class)
    {
        DVM_Class *pNext = SearchClassFromExecutable(pExecutable, pPos->super_class->name);
        iIndex = SetFieldTypes(pExecutable, pNext, ppFieldType, iIndex);
    }

    for (int i = 0; i < pPos->field_count; i++)
    {
        ppFieldType[iIndex] = pPos->field[i].type;
        iIndex++;
    }

    return iIndex;
}

void Load::AddFields(DVM_Executable *pExecutable, DVM_Class *pSrc, ExecClass *pDest)
{
    int iFieldCount = 0;

    for (DVM_Class *pos = pSrc; ; )
    {
        iFieldCount += pos->field_count;

        if (nullptr == pos->super_class)
            break;

        pos = SearchClassFromExecutable(pExecutable, pos->super_class->name);
    }

    pDest->field_count = iFieldCount;
    pDest->field_type = (DVM_TypeSpecifier**)MEM_malloc(sizeof(DVM_TypeSpecifier*) * iFieldCount);

    SetFieldTypes(pExecutable, pSrc, pDest->field_type, 0);
}

void Load::SetVTable(DVM_VirtualMachine *pVirtualMachine, DVM_Class *pClass, DVM_Method *pSrc, VTableItem *pDest, bool bSetName)
{
    if (bSetName)
    {
        pDest->name = MEM_strdup(pSrc->name);
    }

    char *lpstrFuncName = m_Util.CreateMethodFunctionName(pClass->name, pSrc->name);
    int iFuncIndex = SearchFunction(pVirtualMachine, pClass->package_name, lpstrFuncName);

    if (FUNCTION_NOT_FOUND == iFuncIndex && DVM_TRUE == pSrc->is_abstract)
    {
        m_Error.CompileError(NO_LINE_NUMBER_PC, FUNCTION_NOT_FOUND_ERR,
            STRING_MESSAGE_ARGUMENT, "name", lpstrFuncName,
            MESSAGE_ARGUMENT_END);
    }

    MEM_free(lpstrFuncName);
    pDest->index = iFuncIndex;
}

int Load::AddMethod(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, DVM_Class *pPos, DVM_VTable *pVTable)
{
    int iSuperMethodCount = 0;

    if (pPos->super_class)
    {
        DVM_Class *pNext = SearchClassFromExecutable(pExecutable, pPos->super_class->name);
        iSuperMethodCount = AddMethod(pVirtualMachine, pExecutable, pNext, pVTable);
    }

    int iMethodCount = iSuperMethodCount;

    for (int i = 0; i < pPos->method_count; i++)
    {
        int j;
        for (j = 0; j < iSuperMethodCount; j++)
        {
            if (std::string(pPos->method[i].name) == pVTable->table[j].name)
            {
                SetVTable(pVirtualMachine, pPos, &pPos->method[i], &pVTable->table[j], false);
                break;
            }
        }

        /* if pPos->method[i].is_override == DVM_TRUE, this method implements interface method. */
        if (j == iSuperMethodCount && DVM_FALSE == pPos->method[i].is_override)
        {
            pVTable->table = (VTableItem*)MEM_realloc(pVTable->table, sizeof(VTableItem)* (iMethodCount + 1));
            SetVTable(pVirtualMachine, pPos, &pPos->method[i], &pVTable->table[iMethodCount], true);

            iMethodCount++;
            pVTable->table_size = iMethodCount;
        }
    }

    return iMethodCount;
}

DVM_VTable* Load::AllocVTable(ExecClass *pExecClass)
{
    DVM_VTable *pVTable = (DVM_VTable*)MEM_malloc(sizeof(DVM_VTable));

    pVTable->exec_class = pExecClass;
    pVTable->table = nullptr;

    return pVTable;
}