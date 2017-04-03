#include "StdAfx.h"
#include "Interface.h"
#include "DikSam.h"
#include "Debug.h"
#include "Memory.h"
#include "Storage.h"
#include "StringLiteral.h"
#include "Exception.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern int  g_iCurrentThreadIndex;

DKC_Compiler* dkc_get_current_compiler(void)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetInterface()->GetCompiler();
}

#ifdef __cplusplus
}
#endif // __cplusplus

std::mutex Interface::m_Mutex;

Interface::Interface(Debug& debug, Memory& memory, Storage& storage, StringLiteral& stringliteral, int iThreadIndex)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Storage(storage)
    , m_StringLiteral(stringliteral)
    , m_pCompiler(nullptr)
    , m_pExe(nullptr)
    , m_iThreadIndex(iThreadIndex)
{
    CreateCompiler();
}

Interface::~Interface()
{

}

void Interface::Compile(FILE *pFile)
{
    extern FILE *yyin;

    m_pCompiler->current_line_number = 1;
    m_pCompiler->input_mode = DKC_FILE_INPUT_MODE;

    yyin = pFile;

    m_pExe = DoCompile(m_pCompiler, nullptr);

    m_StringLiteral.Reset();
}

void Interface::Compile(char **ppLines)
{
    m_pCompiler->current_line_number = 1;
    m_pCompiler->input_mode = DKC_STRING_INPUT_MODE;

    m_pExe = DoCompile(m_pCompiler, ppLines);

    m_StringLiteral.Reset();
}

void Interface::DisposeCompiler()
{
    FunctionDefinition *pos = m_pCompiler->function_list;

    for (; pos; pos = pos->next)
    {
        m_Memory.Free(pos->local_variable);
    }

    m_Storage.Dispose(m_pCompiler->compile_storage);
}

void Interface::CreateCompiler()
{
    MEM_Storage pStorage = INTERFACE_STORAGE_OPEN(0);
    
    m_pCompiler = (DKC_Compiler*)INTERFACE_STORAGE_MALLOC(pStorage, sizeof(struct DKC_Compiler_tag));
    m_pCompiler->compile_storage = pStorage;
    m_pCompiler->function_list = nullptr;
    m_pCompiler->function_count = 0;
    m_pCompiler->declaration_list = nullptr;
    m_pCompiler->statement_list = nullptr;
    m_pCompiler->current_block = nullptr;
    m_pCompiler->current_line_number = 1;
    m_pCompiler->input_mode = DKC_STRING_INPUT_MODE;
    
#ifdef EUC_SOURCE
    m_pCompiler->source_encoding = EUC_ENCODING;
#else
#ifdef GB2312_SOURCE
    m_pCompiler->source_encoding = GB2312_ENCODING;
#else
#ifdef UTF_8_SOURCE
    m_pCompiler->source_encoding = UTF_8_ENCODING;
#else
    ERROR_DBG_Panic(("source encoding is not defined."));
#endif
#endif
#endif
}

DVM_Executable* Interface::DoCompile(DKC_Compiler *pCompiler, char **ppLines)
{
    extern int yyparse(void);
    extern void ResetLex(void);
    
    do
    {
        m_Mutex.lock();
        g_iCurrentThreadIndex = m_iThreadIndex;

        if (DKC_STRING_INPUT_MODE == m_pCompiler->input_mode)
        {
            dkc_set_source_string(ppLines);
        }

        if (yyparse())
        {
            throw ErrorException(TEXT("Error ! Error ! Error !"));
        }
        ResetLex();
        m_Mutex.unlock();
    } while (0);

    return nullptr;
}