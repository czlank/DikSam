#include "StdAfx.h"
#include "Interface.h"
#include "DikSam.h"
#include "Debug.h"
#include "Memory.h"
#include "Storage.h"
#include "Util.h"
#include "Error.h"
#include "StringLiteral.h"
#include "Create.h"
#include "Load.h"
#include "Exception.h"
#include "FixTree.h"
#include "Generate.h"
#include "Execute.h"
#include "Dispose.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern int  g_iCurrentThreadIndex;

DKC_Compiler* dkc_get_current_compiler(void)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetInterface()->GetCurrentCompiler();
}

#ifdef __cplusplus
}
#endif // __cplusplus

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

#ifdef MEM_free
#undef MEM_free
#endif
#define MEM_free(ptr)                       (m_Memory.Free(ptr))

#ifdef MEM_strdup
#undef MEM_strdup
#endif
#define MEM_strdup(ptr)                     (m_Memory.StrDUP(__FILE__, __LINE__, ptr))

#ifdef MEM_open_sotrage
#undef MEM_open_storage
#endif
#define MEM_open_sotrage(size)              (m_Storage.Open(__FILE__, __LINE__, size))

#ifdef MEM_storage_malloc
#undef MEM_storage_malloc
#endif
#define MEM_storage_malloc(storage, size)   (m_Storage.Malloc(__FILE__, __LINE__, storage, size))

#ifdef dkc_malloc
#undef dkc_malloc
#endif
#define dkc_malloc(size)                    (m_Util.Malloc(__FILE__, __LINE__, size))

std::mutex Interface::m_Mutex;

Interface::BuiltInMethodParameter Interface::m_ArrayResizeArg[] =
{
    { "new_size", DVM_INT_TYPE }
};

Interface::BuiltInMethodParameter Interface::m_ArrayInsertArg[] =
{
    { "index", DVM_INT_TYPE },
    { "new_item", DVM_BASE_TYPE }
};

Interface::BuiltInMethodParameter Interface::m_ArrayRemoveArg[] =
{
    { "index", DVM_INT_TYPE }
};

Interface::BuiltInMethodParameter Interface::m_ArrayAddArg[] =
{
    { "new_item", DVM_BASE_TYPE }
};

Interface::BuiltInMethod Interface::m_ArrayMethod[] =
{
    { ARRAY_METHOD_SIZE, DVM_INT_TYPE, nullptr, 0 },
    { ARRAY_METHOD_RESIZE, DVM_VOID_TYPE, Interface::m_ArrayResizeArg, ARRAY_SIZE(Interface::m_ArrayResizeArg) },
    { ARRAY_METHOD_INSERT, DVM_VOID_TYPE, Interface::m_ArrayInsertArg, ARRAY_SIZE(Interface::m_ArrayInsertArg) },
    { ARRAY_METHOD_REMOVE, DVM_VOID_TYPE, Interface::m_ArrayRemoveArg, ARRAY_SIZE(Interface::m_ArrayRemoveArg) },
    { ARRAY_METHOD_ADD, DVM_VOID_TYPE, Interface::m_ArrayAddArg, ARRAY_SIZE(Interface::m_ArrayAddArg) }
};

Interface::BuiltInMethodParameter Interface::m_StringSubstrArg[] =
{
    { "start", DVM_INT_TYPE },
    { "length", DVM_INT_TYPE }
};

Interface::BuiltInMethod Interface::m_StringMethod[] =
{
    { STRING_METHOD_LENGTH, DVM_INT_TYPE, nullptr, 0 },
    { STRING_METHOD_SUBSTR, DVM_STRING_TYPE, Interface::m_StringSubstrArg, ARRAY_SIZE(Interface::m_StringSubstrArg) }
};

Interface::Interface(Debug& debug, Memory& memory, Storage& storage, Util& util, Error& error, StringLiteral& stringliteral, Create& create, int iThreadIndex)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Storage(storage)
    , m_Util(util)
    , m_Error(error)
    , m_StringLiteral(stringliteral)
    , m_Create(create)
    , m_pCompiler(nullptr)
    , m_pCompilerList(nullptr)
    , m_iThreadIndex(iThreadIndex)
{
}

Interface::~Interface()
{
}

DKC_Compiler* Interface::CreateCompiler()
{
    DKC_Compiler* pCompilerBackup = m_pCompiler;
    MEM_Storage pStorage = MEM_open_sotrage(0);

    m_pCompiler = (DKC_Compiler*)MEM_storage_malloc(pStorage, sizeof(struct DKC_Compiler_tag));

    m_pCompiler->compile_storage = pStorage;
    m_pCompiler->package_name = nullptr;
    m_pCompiler->source_suffix = DKM_SOURCE;
    m_pCompiler->require_list = nullptr;
    m_pCompiler->rename_list = nullptr;
    m_pCompiler->function_list = nullptr;
    m_pCompiler->dvm_function_count = 0;
    m_pCompiler->dvm_function = nullptr;
    m_pCompiler->dvm_class_count = 0;
    m_pCompiler->dvm_class = nullptr;
    m_pCompiler->declaration_list = nullptr;
    m_pCompiler->statement_list = nullptr;
    m_pCompiler->class_definition_list = nullptr;
    m_pCompiler->current_block = nullptr;
    m_pCompiler->current_line_number = 1;
    m_pCompiler->current_class_definition = nullptr;
    m_pCompiler->current_catch_clause = nullptr;
    m_pCompiler->input_mode = STRING_INPUT_MODE;
    m_pCompiler->required_list = nullptr;
    m_pCompiler->array_method_count = ARRAY_SIZE(m_ArrayMethod);
    m_pCompiler->array_method = CreateBuiltInMethod(m_ArrayMethod, ARRAY_SIZE(m_ArrayMethod));
    m_pCompiler->string_method_count = ARRAY_SIZE(m_StringMethod);
    m_pCompiler->string_method = CreateBuiltInMethod(m_StringMethod, ARRAY_SIZE(m_StringMethod));

#ifdef EUC_SOURCE
    m_pCompiler->source_encoding = EUC_ENCODING;
#else
#ifdef GB2312_SOURCE
    m_pCompiler->source_encoding = GB2312_ENCODING;
#else
#ifdef UTF_8_SOURCE
    m_pCompiler->source_encoding = UTF_8_ENCODING;
#else
    DBG_panic(("source encoding is not defined."));
#endif
#endif
#endif

    DKC_Compiler *pCompilerNew = m_pCompiler;
    m_pCompiler = pCompilerBackup;

    return pCompilerNew;
}

void Interface::DisposeCompiler(DKC_Compiler *pCompiler)
{
    CompilerList *pList = TraversalCompiler(nullptr, pCompiler);

    for (CompilerList *pos = pList; pos;)
    {
        for (FunctionDefinition *pFdPos = pos->compiler->function_list; pFdPos; pFdPos = pFdPos->next)
        {
            m_Memory.Free(pFdPos->local_variable);
        }

        CompilerList *pTemp = nullptr;
        while (pos->compiler->required_list)
        {
            pTemp = pos->compiler->required_list;
            pos->compiler->required_list = pTemp->next;
            m_Memory.Free(pTemp);
        }

        m_Storage.Dispose(pos->compiler->compile_storage);
        pTemp = pos->next;
        m_Memory.Free(pos);
        pos = pTemp;
    }
}

void Interface::RunScript(FILE *pFile, const char *lpstrPath)
{
    try
    {
        m_MemoryDump.open("./MemoryDump.dmp", std::ofstream::out | std::ofstream::trunc);
    
        DKC_Compiler *pCompiler = CreateCompiler();
        DVM_ExecutableList *pExecutableList = Compile(pCompiler, pFile, lpstrPath);
        
        Load load(m_Debug, m_Memory, m_Util, m_Error, *this);
        DVM_VirtualMachine *pVirtualMachine = load.CreateVirtualMachine();
        load.SetExecutable(pVirtualMachine, pExecutableList);
        DisposeCompiler(pCompiler);

        //Execute(m_Debug, m_Memory, m_Error)(pExecutable);

        ResetCompiler();

        m_Memory.CheckAllBlocks();
        m_Memory.DumpBlocks(m_MemoryDump);

        m_MemoryDump.flush();
        m_MemoryDump.close();
    }
    catch (const PanicException& e)
    {
        std::cout << e.what();
    }
    catch (const AssertException& e)
    {
        std::cout << e.what();
    }
    catch (const MemoryException& e)
    {
        std::cout << e.what();
    }
    catch (const ErrorException& e)
    {
        std::cout << e.what();
    }
}

void Interface::RunScript(char **ppLines, const char *lpstrPath)
{
    try
    {
        m_MemoryDump.open("./MemoryDump.dmp", std::ofstream::out | std::ofstream::trunc);

        DKC_Compiler *pCompiler = CreateCompiler();
        DVM_ExecutableList *pExecutableList = Compile(pCompiler, ppLines, lpstrPath);

        Load load(m_Debug, m_Memory, m_Util, m_Error, *this);
        DVM_VirtualMachine *pVirtualMachine = load.CreateVirtualMachine();
        load.SetExecutable(pVirtualMachine, pExecutableList);
        DisposeCompiler(pCompiler);

        //Execute(m_Debug, m_Memory, m_Error)(pExecutable);

        ResetCompiler();

        m_Memory.CheckAllBlocks();
        m_Memory.DumpBlocks(m_MemoryDump);

        m_MemoryDump.flush();
        m_MemoryDump.close();
    }
    catch (const PanicException& e)
    {
        std::cout << e.what();
    }
    catch (const AssertException& e)
    {
        std::cout << e.what();
    }
    catch (const MemoryException& e)
    {
        std::cout << e.what();
    }
    catch (const ErrorException& e)
    {
        std::cout << e.what();
    }
}

SearchFileStatus Interface::DynamicCompile(DKC_Compiler *pCompiler, char *lpstrPackageName, DVM_ExecutableList *pExecutableList, DVM_ExecutableItem **ppAddTop, char *lpstrSearchFile)
{
    extern FILE *yyin;

    char chFoundPath[FILENAME_MAX]{};
    SourceInput sourceInput;
    SearchFileStatus status = GetDynamicLoadInput(lpstrPackageName, chFoundPath, lpstrSearchFile, &sourceInput);

    if (status != SEARCH_FILE_SUCCESS)
    {
        return status;
    }

    DBG_assert(nullptr == m_pCompilerList, ("m_pCompilerList != nullptr(", int(m_pCompilerList), ")"));

    DVM_ExecutableItem *pTail = pExecutableList->list;
    for (; pTail->next; pTail = pTail->next)
        ;

    pCompiler->package_name = StringToPackageName(pCompiler, lpstrPackageName);
    SetPathToCompiler(pCompiler, chFoundPath);

    pCompiler->input_mode = sourceInput.input_mode;
    if (FILE_INPUT_MODE == sourceInput.input_mode)
    {
        yyin = sourceInput.u.file.fp;
        (void)DoCompile(pCompiler, nullptr, pExecutableList, chFoundPath, false);
    }
    else
    {
        dkc_set_source_string(sourceInput.u.string.lines);
        (void)DoCompile(pCompiler, sourceInput.u.string.lines, pExecutableList, chFoundPath, false);
    }

    DisposeCompilerList();
    m_StringLiteral.Reset();

    *ppAddTop = pTail->next;

    return SEARCH_FILE_SUCCESS;
}

DVM_ExecutableList* Interface::Compile(DKC_Compiler *pCompiler, FILE *pFile, const char *lpstrPath)
{
    extern FILE *yyin;

    DBG_assert(nullptr == m_pCompilerList, ("m_pCompilerList != nullptr(", (int)m_pCompilerList, ")"));

    SetPathToCompiler(pCompiler, lpstrPath);
    pCompiler->input_mode = FILE_INPUT_MODE;

    yyin = pFile;

    DVM_ExecutableList *pList = (DVM_ExecutableList*)MEM_malloc(sizeof(DVM_ExecutableList));
    pList->list = nullptr;

    DVM_Executable *pExecutable = DoCompile(pCompiler, nullptr, pList, nullptr, false);
    pExecutable->path = MEM_strdup(lpstrPath);
    pList->top_level = pExecutable;

    DisposeCompilerList();
    m_StringLiteral.Reset();

    return pList;
}

DVM_ExecutableList* Interface::Compile(DKC_Compiler *pCompiler, char **ppLines, const char *lpstrPath)
{
    DBG_assert(nullptr == m_pCompilerList, ("m_pCompilerList != nullptr(", (int)m_pCompilerList, ")"));

    SetPathToCompiler(pCompiler, lpstrPath);
    pCompiler->input_mode = STRING_INPUT_MODE;

    DVM_ExecutableList *pList = (DVM_ExecutableList*)MEM_malloc(sizeof(DVM_ExecutableList));
    pList->list = nullptr;

    DVM_Executable *pExecutable = DoCompile(pCompiler, ppLines, pList, nullptr, false);
    pExecutable->path = MEM_strdup(lpstrPath);
    pList->top_level = pExecutable;

    DisposeCompilerList();
    m_StringLiteral.Reset();

    return pList;
}

DVM_Executable* Interface::DoCompile(DKC_Compiler *pCompiler, char **ppLines, DVM_ExecutableList *pExecutableList, const char *lpstrPath, bool isRequired)
{
    extern FILE *yyin;
    extern int yyparse(void);
    extern void ResetLex(void);

    DKC_Compiler* pCompilerBackup = m_pCompiler;
    m_pCompiler = pCompiler;

    do
    {
        m_Mutex.lock();
        g_iCurrentThreadIndex = m_iThreadIndex;

        if (STRING_INPUT_MODE == m_pCompiler->input_mode)
        {
            dkc_set_source_string(ppLines);
        }

        try
        {
            if (yyparse())
            {
                ResetLex();
                m_Mutex.unlock();
                throw ErrorException(TEXT("Error ! Error ! Error !"));
            }
        }
        catch (const ErrorException& e)
        {
            ResetLex();
            m_Mutex.unlock();
            throw e;
        }

        ResetLex();
        m_Mutex.unlock();
    } while (0);

    for (RequireList *pReqPos = pCompiler->require_list; pReqPos; pReqPos = pReqPos->next)
    {
        DKC_Compiler *pReqCompiler = SearchCompiler(m_pCompilerList, pReqPos->package_name);

        if (pReqCompiler)
        {
            pCompiler->required_list = AddCompilerToList(pCompiler->required_list, pReqCompiler);
            continue;
        }

        pReqCompiler = CreateCompiler();

        pReqCompiler->package_name = pReqPos->package_name;
        pReqCompiler->source_suffix = DKH_SOURCE;

        pCompiler->required_list = AddCompilerToList(pCompiler->required_list, pReqCompiler);
        m_pCompilerList = AddCompilerToList(m_pCompilerList, pReqCompiler);

        char chFoundPath[FILENAME_MAX]{};
        SourceInput sourceInput;

        GetRequireInput(pReqPos, chFoundPath, &sourceInput);
        SetPathToCompiler(pReqCompiler, chFoundPath);

        pReqCompiler->input_mode = sourceInput.input_mode;

        if (FILE_INPUT_MODE == sourceInput.input_mode)
        {
            yyin = sourceInput.u.file.fp;
        }
        else
        {
            dkc_set_source_string(sourceInput.u.string.lines);
        }

        (void)DoCompile(pReqCompiler, sourceInput.u.string.lines, pExecutableList, chFoundPath, true);
    }

    FixTree(m_Debug, m_Memory, m_Util, m_Error, m_Create, *this)(pCompiler);
    DVM_Executable *pExecutable = Generate(m_Debug, m_Memory, m_Util, m_Error)(pCompiler);

    if (lpstrPath)
    {
        pExecutable->path = MEM_strdup(lpstrPath);
    }
    else
    {
        pExecutable->path = nullptr;
    }

    pExecutable->is_required = isRequired ? DVM_TRUE : DVM_FALSE;

    if (!AddExeToList(pExecutable, pExecutableList))
    {
        Dispose(m_Debug, m_Memory)(pExecutable);
    }

    m_pCompiler = pCompilerBackup;
    return pExecutable;
}

void Interface::ResetCompiler()
{
    m_StringLiteral.Reset();
    m_Memory.FreeLiteralPool();
}

FunctionDefinition* Interface::CreateBuiltInMethod(BuiltInMethod *pMethod, int iMethodCount)
{
    FunctionDefinition *pFunctionDefinitionArray = (FunctionDefinition*)dkc_malloc(sizeof(FunctionDefinition) * iMethodCount);

    for (int i = 0; i < iMethodCount; i++)
    {
        pFunctionDefinitionArray[i].name = pMethod[i].name;
        pFunctionDefinitionArray[i].type = m_Util.AllocTypeSpecifier(pMethod[i].return_type);

        ParameterList *pParameterList = nullptr;

        for (int idx = 0; idx < pMethod[i].parameter_count; idx++)
        {
            TypeSpecifier *pType = m_Util.AllocTypeSpecifier(pMethod[i].parameter[idx].type);

            if (pParameterList)
            {
                pParameterList = m_Create.ChainParameter(pParameterList, pType, pMethod[i].parameter[idx].name);
            }
            else
            {
                pParameterList = m_Create.CreateParameter(pType, pMethod[i].parameter[idx].name);
            }
        }

        pFunctionDefinitionArray[i].parameter = pParameterList;
    }

    return pFunctionDefinitionArray;
}

void Interface::SetPathToCompiler(DKC_Compiler *pCompiler, const char *lpstrPath)
{
    DBG_assert(lpstrPath, ("lpstrPath is nullptr"));

    int iLen = std::string(lpstrPath).length();
    pCompiler->path = (char*)MEM_storage_malloc(pCompiler->compile_storage, iLen + 1);

    for (int i = 0; i < iLen; i++)
    {
        pCompiler->path[i] = *(lpstrPath + i);
    }

    pCompiler->path[iLen] = '\0';
}

void Interface::DisposeCompilerList()
{
    while (m_pCompilerList)
    {
        CompilerList *pTemp = m_pCompilerList;
        m_pCompilerList = pTemp->next;
        MEM_free(pTemp);
    }
}

DKC_Compiler* Interface::SearchCompiler(CompilerList *pList, PackageName *pPackageName)
{
    CompilerList *pos = pList;

    for (; pos; pos = pos->next)
    {
        if (m_Util.ComparePackageName(pos->compiler->package_name, pPackageName))
            break;
    }

    if (pos)
    {
        return pos->compiler;
    }
    else
    {
        return nullptr;
    }
}

CompilerList* Interface::AddCompilerToList(CompilerList *pList, DKC_Compiler *pCompilter)
{
    CompilerList *pNewList = (CompilerList*)MEM_malloc(sizeof(CompilerList));

    pNewList->compiler = pCompilter;
    pNewList->next = nullptr;

    if (nullptr == pList)
    {
        return pNewList;
    }
    
    CompilerList *pos = pList;

    for (; pos->next; pos = pos->next)
        ;

    pos->next = pNewList;

    return pList;
}

void Interface::GetRequireInput(RequireList *pReqList, char *lpstrFoundPath, SourceInput *pSourceInput)
{
    char *lpstrSearchPath = nullptr;
    size_t len = 0;
    errno_t err = _dupenv_s(&lpstrSearchPath, &len, "DKM_REQUIRE_SEARCH_PATH");

    if (err)
    {
        if (lpstrSearchPath)
        {
            free(lpstrSearchPath);
        }

        DBG_assert(0, ("can't find environment variable DKM_REQUIRE_SEARCH_PATH, err = ", err));
    }

    bool bLiteralPool = false;
    if (nullptr == lpstrSearchPath)
    {
        lpstrSearchPath = ".";
        bLiteralPool = true;
    }

    char chSearchFile[FILENAME_MAX]{};
    MakeSearchPath(pReqList->line_number, pReqList->package_name, chSearchFile);

    FILE *fp = nullptr;
    SearchFileStatus status = m_Util.SearchFile(lpstrSearchPath, chSearchFile, lpstrFoundPath, &fp);

    if (status != SEARCH_FILE_SUCCESS)
    {
        if (SEARCH_FILE_NOT_FOUND == status)
        {
            m_Error.CompileError(pReqList->line_number, REQUIRE_FILE_NOT_FOUND_ERR, STRING_MESSAGE_ARGUMENT, "file", chSearchFile, MESSAGE_ARGUMENT_END);
        }
        else
        {
            m_Error.CompileError(pReqList->line_number, REQUIRE_FILE_ERR, INT_MESSAGE_ARGUMENT, "status", (int)status, MESSAGE_ARGUMENT_END);
        }
    }

    pSourceInput->input_mode = FILE_INPUT_MODE;
    pSourceInput->u.file.fp = fp;

    if (!bLiteralPool)
    {
        free(lpstrSearchPath);
    }
}

bool Interface::AddExeToList(DVM_Executable *pExecutable, DVM_ExecutableList *pList)
{
    DVM_ExecutableItem *pTail = nullptr;

    for (DVM_ExecutableItem *pos = pList->list; pos; pos = pos->next)
    {
        if (m_Util.ComparePackageName(pos->executable->package_name, pExecutable->package_name)
            && pos->executable->is_required == pExecutable->is_required)
        {
            return false;
        }

        pTail = pos;
    }

    DVM_ExecutableItem *pNewItem = (DVM_ExecutableItem*)MEM_malloc(sizeof(DVM_ExecutableItem));

    pNewItem->executable = pExecutable;
    pNewItem->next = nullptr;

    if (nullptr == pList->list)
    {
        pList->list = pNewItem;
    }
    else
    {
        pTail->next = pNewItem;
    }

    return true;
}

void Interface::MakeSearchPath(int iLineNumber, PackageName *pPackageName, char *pBuf)
{
    int iSuffixLen = std::string(DIKSAM_REQUIRE_SUFFIX).length();
    
    int iLen = 0;
    int iPrevLen = 0;

    pBuf[0] = '\0';

    for (PackageName *pos = pPackageName; pos; pos = pos->next)
    {
        iPrevLen = iLen;
        iLen += std::string(pos->name).length();

        if (iLen > FILENAME_MAX - (2 + iSuffixLen))
        {
            m_Error.CompileError(iLineNumber, PACKAGE_NAME_TOO_LONG_ERR, MESSAGE_ARGUMENT_END);
        }

        for (size_t i = 0; i < std::string(pos->name).length(); i++)
        {
            pBuf[iPrevLen + i] = pos->name[i];
        }

        if (pos->next)
        {
            pBuf[std::string(pBuf).length()] = FILE_SEPARATOR;
        }
    }

    std::string sRequireSuffix(DIKSAM_REQUIRE_SUFFIX);
    for (size_t i = 0; i < sRequireSuffix.length(); i++)
    {
        pBuf[iLen + i] = sRequireSuffix[i];
    }
}

void Interface::MakeSearchPathImpl(char *lpstrPackageName, char *pBuf)
{
    int iSuffixLen = std::string(DIKSAM_IMPLEMENTATION_SUFFIX).length();
    int iPackageLen = std::string(lpstrPackageName).length();

    DBG_assert(iPackageLen <= FILENAME_MAX - (2 + iSuffixLen), ("package name is too long(", lpstrPackageName, ")"));

    int i = 0;
    for (; lpstrPackageName[i] != '\0'; i++)
    {
        if ('.' == lpstrPackageName[i])
        {
            pBuf[i] = FILE_SEPARATOR;
        }
        else
        {
            pBuf[i] = lpstrPackageName[i];
        }
    }

    std::string sSuffix(DIKSAM_IMPLEMENTATION_SUFFIX);

    for (size_t idx = 0; idx < sSuffix.length(); idx++, i++)
    {
        pBuf[i] = sSuffix[idx];
    }

    pBuf[i] = '\0';
}

PackageName* Interface::CreateOnePackageName(DKC_Compiler *pCompiler, char *str, int iStartIdx, int iToIdx)
{
    MEM_Storage storage = pCompiler->compile_storage;
    PackageName *pPackageName = (PackageName*)MEM_storage_malloc(storage, sizeof(PackageName));
    
    pPackageName->name = (char*)MEM_storage_malloc(storage, iToIdx - iStartIdx + 1);

    int i = 0;
    for (; i < iToIdx - iStartIdx; i++)
    {
        pPackageName->name[i] = str[iStartIdx + i];
    }

    pPackageName->name[i] = '\0';
    pPackageName->next = nullptr;

    return pPackageName;
}

PackageName* Interface::StringToPackageName(DKC_Compiler *pCompiler, char *str)
{
    int i;
    int iStartIdx;

    PackageName *pPackageName = nullptr;
    PackageName *pTop = nullptr;
    PackageName *pTail = nullptr;

    for (i = 0, iStartIdx = 0; str[i] != '\0'; i++)
    {
        if ('.' == str[i])
        {
            pPackageName = CreateOnePackageName(pCompiler, str, iStartIdx, i);
            iStartIdx = i + 1;

            if (pTail)
            {
                pTail->next = pPackageName;
            }
            else
            {
                pTop = pPackageName;
            }

            pTail = pPackageName;
        }
    }

    pPackageName = CreateOnePackageName(pCompiler, str, iStartIdx, i);

    if (pTail)
    {
        pTail->next = pPackageName;
    }
    else
    {
        pTop = pPackageName;
    }

    return pTop;
}

SearchFileStatus Interface::GetDynamicLoadInput(char *lpstrPackageName, char *lpstrFoundPath, char *lpstrSearchFile, SourceInput *pSourceInput)
{
    char *lpstrSearchPath = nullptr;
    size_t len = 0;
    errno_t err = _dupenv_s(&lpstrSearchPath, &len, "DKM_LOAD_SEARCH_PATH");

    if (err)
    {
        if (lpstrSearchPath)
        {
            free(lpstrSearchPath);
        }

        DBG_assert(0, ("can't find environment variable DKM_LOAD_SEARCH_PATH, err = ", err));
    }

    bool bLiteralPool = false;
    if (nullptr == lpstrSearchPath)
    {
        lpstrSearchPath = ".";
        bLiteralPool = true;
    }

    MakeSearchPathImpl(lpstrPackageName, lpstrSearchFile);

    FILE *fp = nullptr;
    SearchFileStatus status = m_Util.SearchFile(lpstrSearchPath, lpstrSearchFile, lpstrFoundPath, &fp);

    if (status != SEARCH_FILE_SUCCESS)
    {
        if (!bLiteralPool)
        {
            free(lpstrSearchPath);
        }

        return status;
    }

    pSourceInput->input_mode = FILE_INPUT_MODE;
    pSourceInput->u.file.fp = fp;

    if (!bLiteralPool)
    {
        free(lpstrSearchPath);
    }

    return SEARCH_FILE_SUCCESS;
}

CompilerList* Interface::TraversalCompiler(CompilerList *pList, DKC_Compiler *pCompiler)
{
    CompilerList *pListPos = nullptr;

    for (pListPos = pList; pListPos; pListPos = pListPos->next)
    {
        if (pListPos->compiler == pCompiler)
            break;
    }

    if (nullptr == pListPos)
    {
        pList = AddCompilerToList(pList, pCompiler);
    }

    for (CompilerList *pReqPos = pCompiler->required_list; pReqPos; pReqPos = pReqPos->next)
    {
        pList = TraversalCompiler(pList, pReqPos->compiler);
    }

    return pList;
}