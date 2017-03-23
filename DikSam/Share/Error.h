#pragma once

class Error
{
public:
    Error();
    ~Error();

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
    int InitMessageFormat();

private:
    typedef std::map<int, const wchar_t*>   MessageFormat, *pMessageFormat;
    pMessageFormat  m_pMessageFormat;
    MessageFormat   m_CompilerErrorMessageFormat;
    MessageFormat   m_DvmErrorMessageFormat;
};