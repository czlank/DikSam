#pragma once

#include "Exception.h"
#include "DVM_pri.h"

class Debug;
class Util;

class Error
{
public:
    Error(Debug& debug, Util& util);
    ~Error();

    void FlexError(int iLine, char *lpstrErrorMessage);
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
    Debug   &m_Debug;
    Util    &m_Util;

    std::map<int, const wchar_t*>   m_ErrorMessageFormat;
};