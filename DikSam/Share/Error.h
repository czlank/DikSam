#pragma once

#include "Exception.h"
#include "Util.h"
#include "Debug.h"
#include "DVM_pri.h"

class Error
{
#define ERROR_DBG_Assert(expression, arg) ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))
#define ERROR_DBG_Panic(arg) (m_Debug.Panic(__FILE__, __LINE__, arg))

public:
    Error(Util& util, Debug& debug);
    ~Error();

    void CompileError(int iLine, DikSamError id, ...);
    void DVMError(DVM_Executable *pExe, Function *pFunc, int pc, DikSamError id, ...);

private:
    typedef struct
    {
        MessageArgumentType     m_enType;
        const char              *m_lpcstrName;
        union
        {
            int                 m_iIntVal;
            double              m_dbDoubleVal;
            char                *m_lpstrStringVal;
            void                *m_pPointerVal;
            int                 m_iCharacterVal;
        } u;
    } MessageArgument;

private:
    void InitMessageFormat();
    void SelfCheck();
    void CreateMessageArgument(std::vector<MessageArgument>& vecArg, va_list ap);
    std::vector<MessageArgument>::iterator SearchArgument(std::vector<MessageArgument>& vecArg, const char* lpcstrName);
    void FormatMessage(const wchar_t* lpcwstrFormat, VString *vstrMsg, va_list ap);
    int ConvertPCToLineNumber(DVM_Executable *pExe, Function *pFunc, int pc);

private:
    Util    &m_Util;
    Debug   &m_Debug;

    std::map<int, const wchar_t*>   m_ErrorMessageFormat;
};