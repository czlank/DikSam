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

DVM_ObjectRef Load::m_NullObjectRef =
{
    nullptr,
    nullptr
};

VTableItem Load::m_stArrayMethodVTable[] =
{
    { ARRAY_PREFIX ARRAY_METHOD_SIZE,   FUNCTION_NOT_FOUND },
    { ARRAY_PREFIX ARRAY_METHOD_RESIZE, FUNCTION_NOT_FOUND },
    { ARRAY_PREFIX ARRAY_METHOD_INSERT, FUNCTION_NOT_FOUND },
    { ARRAY_PREFIX ARRAY_METHOD_REMOVE, FUNCTION_NOT_FOUND },
    { ARRAY_PREFIX ARRAY_METHOD_ADD,    FUNCTION_NOT_FOUND }
};

VTableItem Load::m_stStringMethodVTable[] =
{
    { STRING_PREFIX STRING_METHOD_LENGTH, FUNCTION_NOT_FOUND },
    { STRING_PREFIX STRING_METHOD_SUBSTR, FUNCTION_NOT_FOUND }
};

Load::Load(Debug& debug, Memory& memory, Util& util, Error& error)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Util(util)
    , m_Error(error)
    , m_Native(debug, memory)
    
{
}

Load::~Load()
{
}

DVM_VirtualMachine* Load::CreateVirtualMachine()
{
    DVM_VirtualMachine *pVirtualMachine = (DVM_VirtualMachine*)MEM_malloc(sizeof(DVM_VirtualMachine));

    pVirtualMachine->stack.alloc_size = STACK_ALLOC_SIZE;
    pVirtualMachine->stack.stack = (DVM_Value*)MEM_malloc(sizeof(DVM_Value) * STACK_ALLOC_SIZE);
    pVirtualMachine->stack.pointer_flags = (DVM_Boolean*)MEM_malloc(sizeof(DVM_Boolean) * STACK_ALLOC_SIZE);
    pVirtualMachine->stack.stack_pointer = 0;

    pVirtualMachine->heap.current_heap_size = 0;
    pVirtualMachine->heap.header = nullptr;
    pVirtualMachine->heap.current_threshold = HEAP_THRESHOLD_SIZE;

    pVirtualMachine->current_executable = nullptr;
    pVirtualMachine->current_function = nullptr;
    pVirtualMachine->current_exception = m_NullObjectRef;

    pVirtualMachine->function = nullptr;
    pVirtualMachine->function_count = 0;

    pVirtualMachine->classes = nullptr;
    pVirtualMachine->class_count = 0;

    pVirtualMachine->executable_list = nullptr;
    pVirtualMachine->executable_entry = nullptr;
    pVirtualMachine->top_level = nullptr;

    m_Native.AddNativeFunctions(pVirtualMachine);
    SetBuiltInMethods(pVirtualMachine);

    return pVirtualMachine;
}

void Load::SetExecutable(DVM_VirtualMachine *pVirtualMachine, DVM_ExecutableList *pList)
{
    pVirtualMachine->executable_list = pList;

    for (DVM_ExecutableItem *pos = pList->list; pos; pos = pos->next)
    {
        (void)AddExecutableToDvm(pVirtualMachine, pos->executable, pos->executable == pList->top_level ? DVM_TRUE : DVM_FALSE);
    }
}

void Load::AddNativeFunction(DVM_VirtualMachine *pVirtualMachine, char *lpstrPackageName, char *lpstrFuncName, DVM_NativeFunctionProc *pProc, int iArgCount, DVM_Boolean bIsMethod, DVM_Boolean bReturnPointer)
{
    pVirtualMachine->function = (Function**)MEM_realloc(pVirtualMachine->function, sizeof(Function*) * (pVirtualMachine->function_count + 1));
    pVirtualMachine->function[pVirtualMachine->function_count] = (Function*)MEM_malloc(sizeof(Function));
    pVirtualMachine->function[pVirtualMachine->function_count]->package_name = MEM_strdup(lpstrFuncName);
    pVirtualMachine->function[pVirtualMachine->function_count]->name = MEM_strdup(lpstrFuncName);
    pVirtualMachine->function[pVirtualMachine->function_count]->kind = NATIVE_FUNCTION;
    pVirtualMachine->function[pVirtualMachine->function_count]->is_implemented = DVM_TRUE;
    pVirtualMachine->function[pVirtualMachine->function_count]->u.native_f.proc = pProc;
    pVirtualMachine->function[pVirtualMachine->function_count]->u.native_f.arg_count = iArgCount;
    pVirtualMachine->function[pVirtualMachine->function_count]->u.native_f.is_method = bIsMethod;
    pVirtualMachine->function[pVirtualMachine->function_count]->u.native_f.return_pointer = bReturnPointer;

    pVirtualMachine->function_count++;
}

void Load::DynamicLoad(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pCallerExecutable, Function *pCaller, int iPC, Function *pFunc)
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

void Load::AddMethods(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, DVM_Class *pSrc, ExecClass *pDest)
{
    DVM_VTable *pVTable = AllocVTable(pDest);
    int iMethodCount = AddMethod(pVirtualMachine, pExecutable, pSrc, pVTable);

    pDest->class_table = pVTable;
    pDest->interface_count = pSrc->interface_count;
    pDest->interface_v_table = (DVM_VTable**)MEM_malloc(sizeof(DVM_VTable*) * pSrc->interface_count);

    for (int i = 0; i < pSrc->interface_count; i++)
    {
        pDest->interface_v_table[i] = AllocVTable(pDest);

        DVM_Class *pInterface = SearchClassFromExecutable(pExecutable, pSrc->interface_[i].name);

        pDest->interface_v_table[i]->table = (VTableItem*)MEM_malloc(sizeof(VTableItem) * pInterface->method_count);
        pDest->interface_v_table[i]->table_size = pInterface->method_count;

        for (int iMethodIndex = 0; iMethodIndex < pInterface->method_count; iMethodIndex++)
        {
            SetVTable(pVirtualMachine, pSrc, &pInterface->method[iMethodIndex], &pDest->interface_v_table[i]->table[iMethodIndex], true);
        }
    }
}

void Load::AddClass(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, DVM_Class *pSrc, ExecClass *pDest)
{
    AddFields(pExecutable, pSrc, pDest);
    AddMethods(pVirtualMachine, pExecutable, pSrc, pDest);
}

void Load::SetSuperClass(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, int iOldClassCount)
{
    for (int iClassIndex = iOldClassCount; iClassIndex < pVirtualMachine->class_count; iClassIndex++)
    {
        DVM_Class *pClass = SearchClassFromExecutable(pExecutable, pVirtualMachine->classes[iClassIndex]->name);

        if (nullptr == pClass->super_class)
        {
            pVirtualMachine->classes[iClassIndex]->super_class = nullptr;
        }
        else
        {
            int iSuperClassIndex = SearchClass(pVirtualMachine, pClass->super_class->package_name, pClass->super_class->name);
            pVirtualMachine->classes[iClassIndex]->super_class = pVirtualMachine->classes[iSuperClassIndex];
        }

        pVirtualMachine->classes[iClassIndex]->interface_ = (ExecClass**)MEM_malloc(sizeof(ExecClass*) * pClass->interface_count);

        for (int index = 0; index < pClass->interface_count; index++)
        {
            int iInterfaceIndex = SearchClass(pVirtualMachine, pClass->interface_[index].package_name, pClass->interface_[index].name);
            pVirtualMachine->classes[iClassIndex]->interface_[index] = pVirtualMachine->classes[iInterfaceIndex];
        }
    }
}

void Load::AddClasses(DVM_VirtualMachine *pVirtualMachine, ExecutableEntry *pExecutableEntry)
{
    int iAddClassCount = 0;
    DVM_Executable *pExecutable = pExecutableEntry->executable;
    DVM_Boolean *pNewClassFlags = (DVM_Boolean*)MEM_malloc(sizeof(DVM_Boolean) * pExecutable->class_count);

    for (int i = 0; i < pExecutable->class_count; i++)
    {
        pNewClassFlags[i] = DVM_FALSE;
    }

    for (int iSrcIndex = 0; iSrcIndex < pExecutable->class_count; iSrcIndex++)
    {
        int iDestIndex = 0;

        for (; iDestIndex < pVirtualMachine->class_count; iDestIndex++)
        {
            if (std::string(pVirtualMachine->classes[iDestIndex]->name) == pExecutable->class_definition[iSrcIndex].name
                && m_Util.ComparePackageName(pVirtualMachine->classes[iDestIndex]->package_name, pExecutable->class_definition[iSrcIndex].package_name))
            {
                if (DVM_TRUE == pExecutable->class_definition[iSrcIndex].is_implemented
                    && DVM_TRUE == pVirtualMachine->classes[iDestIndex]->is_implemented)
                {
                    char *lpstrPackageName = "";

                    if (pVirtualMachine->classes[iDestIndex]->package_name)
                    {
                        lpstrPackageName = pVirtualMachine->classes[iDestIndex]->package_name;
                    }

                    m_Error.CompileError(NO_LINE_NUMBER_PC, CLASS_MULTIPLE_DEFINE_ERR,
                        STRING_MESSAGE_ARGUMENT, "package", lpstrPackageName,
                        STRING_MESSAGE_ARGUMENT, "name", pVirtualMachine->classes[iDestIndex]->name,
                        MESSAGE_ARGUMENT_END);
                }

                pNewClassFlags[iSrcIndex] = DVM_FALSE;

                if (DVM_TRUE == pExecutable->class_definition[iSrcIndex].is_implemented)
                {
                    AddClass(pVirtualMachine, pExecutable, &pExecutable->class_definition[iSrcIndex], pVirtualMachine->classes[iDestIndex]);
                }

                break;
            }
        }

        if (iDestIndex == pVirtualMachine->class_count)
        {
            pNewClassFlags[iSrcIndex] = DVM_TRUE;
            iAddClassCount++;
        }
    }

    pVirtualMachine->classes = (ExecClass**)MEM_realloc(pVirtualMachine->classes, sizeof(ExecClass*) * (pVirtualMachine->class_count + iAddClassCount));

    for (int iSrcIndex = 0, iDestIndex = pVirtualMachine->class_count; iSrcIndex < pExecutable->class_count; iSrcIndex++)
    {
        if (DVM_FALSE == pNewClassFlags[iSrcIndex])
            continue;

        pVirtualMachine->classes[iDestIndex] = (ExecClass*)MEM_malloc(sizeof(ExecClass));
        pVirtualMachine->classes[iDestIndex]->dvm_class = &pExecutable->class_definition[iSrcIndex];
        pVirtualMachine->classes[iDestIndex]->executable = pExecutableEntry;

        if (pExecutable->class_definition[iSrcIndex].package_name)
        {
            pVirtualMachine->classes[iDestIndex]->package_name = MEM_strdup(pExecutable->class_definition[iSrcIndex].package_name);
        }
        else
        {
            pVirtualMachine->classes[iDestIndex]->package_name = nullptr;
        }

        pVirtualMachine->classes[iDestIndex]->name = MEM_strdup(pExecutable->class_definition[iSrcIndex].name);
        pVirtualMachine->classes[iDestIndex]->is_implemented = pExecutable->class_definition[iSrcIndex].is_implemented;
        pVirtualMachine->classes[iDestIndex]->class_index = iDestIndex;

        if (DVM_TRUE == pExecutable->class_definition[iSrcIndex].is_implemented)
        {
            AddClass(pVirtualMachine, pExecutable, &pExecutable->class_definition[iSrcIndex], pVirtualMachine->classes[iDestIndex]);
        }

        iDestIndex++;
    }

    int iOldClassCount = pVirtualMachine->class_count;
    pVirtualMachine->class_count += iAddClassCount;

    SetSuperClass(pVirtualMachine, pExecutable, iOldClassCount);

    MEM_free(pNewClassFlags);
}

ExecutableEntry* Load::AddExecutableToDvm(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, DVM_Boolean bIsTopLevel)
{
    ExecutableEntry *pNewEntry = (ExecutableEntry*)MEM_malloc(sizeof(ExecutableEntry));

    pNewEntry->executable = pExecutable;
    pNewEntry->next = nullptr;

    if (nullptr == pVirtualMachine->executable_entry)
    {
        pVirtualMachine->executable_entry = pNewEntry;
    }
    else
    {
        ExecutableEntry *pos = pVirtualMachine->executable_entry;

        for (; pos->next; pos = pos->next)
            ;

        pos->next = pNewEntry;
    }

    AddFunctions(pVirtualMachine, pNewEntry);
    AddClasses(pVirtualMachine, pNewEntry);

    ConvertCode(pVirtualMachine, pExecutable, pExecutable->code, pExecutable->code_size, nullptr);

    for (int i = 0; i < pExecutable->function_count; i++)
    {
        if (DVM_TRUE == pExecutable->function[i].is_implemented)
        {
            ConvertCode(pVirtualMachine, pExecutable, pExecutable->function[i].code, pExecutable->function[i].code_size, &pExecutable->function[i]);
        }
    }

    AddStaticVariables(pNewEntry, pExecutable);

    if (DVM_TRUE == bIsTopLevel)
    {
        pVirtualMachine->top_level = pNewEntry;
    }

    return pNewEntry;
}

void Load::SetBuiltInMethods(DVM_VirtualMachine *pVirtualMachine)
{
    DVM_VTable *pArrayVTable = AllocVTable(nullptr);
    
    pArrayVTable->table_size = ARRAY_SIZE(m_stArrayMethodVTable);
    pArrayVTable->table = (VTableItem*)MEM_malloc(sizeof(VTableItem) * pArrayVTable->table_size);

    for (int i = 0; i < pArrayVTable->table_size; i++)
    {
        pArrayVTable->table[i] = m_stArrayMethodVTable[i];
        pArrayVTable->table[i].index = SearchFunction(pVirtualMachine, BUILT_IN_METHOD_PACKAGE_NAME, pArrayVTable->table[i].name);
    }

    pVirtualMachine->array_v_table = pArrayVTable;

    DVM_VTable *pStringVTable = AllocVTable(nullptr);

    pStringVTable->table_size = ARRAY_SIZE(m_stStringMethodVTable);
    pStringVTable->table = (VTableItem*)MEM_malloc(sizeof(VTableItem) * pStringVTable->table_size);

    for (int i = 0; i < pStringVTable->table_size; i++)
    {
        pStringVTable->table[i] = m_stStringMethodVTable[i];
        pStringVTable->table[i].index = SearchFunction(pVirtualMachine, BUILT_IN_METHOD_PACKAGE_NAME, pStringVTable->table[i].name);
    }

    pVirtualMachine->string_v_table = pStringVTable;
}