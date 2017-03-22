#pragma once

#include "Exception.h"

class Debug
{
public:
    Debug();
    ~Debug();

    void SetLevel(int iLevel) { m_PanicLevel = iLevel; }

    template <typename T, typename ... Args> void Assert(const char *lpcstrFileName, int iLine, const char *lpcstrExpression, T val, Args && ... args)
    {
        if (0 == m_PanicLevel)
        {
            std::stringstream ss;
            m_Error = "Assertion failure ";

            m_Error += lpcstrExpression;
            m_Error += " file..";
            
            std::string strFileName(lpcstrFileName);
            size_t npos = strFileName.find_last_of('\\');
            assert(npos != std::string::npos);
            strFileName = strFileName.substr(npos + 1, strFileName.length() - (npos + 1));

            m_Error += strFileName + " line..";
            ss << iLine;
            m_Error += ss.str() + "    ";
        }

        std::stringstream ss;
        ss << val;
        m_Error += ss.str();

        Assert(lpcstrFileName, iLine, lpcstrExpression, std::forward<Args&&>(args)...);
    }

    void Assert(const char *lpcstrFileName, int iLine, const char *lpcstrExpression)
    {
        m_PanicLevel = 0;
        throw AssertException(m_Error.c_str());
    }

    template <typename T, typename ... Args> void Panic(const char *lpcstrFileName, int iLine, T val, Args && ... args)
    {
        if (0 == m_PanicLevel)
        {
            std::stringstream ss;
            m_Error = "Panic!! file..";
            
            std::string strFileName(lpcstrFileName);
            size_t npos = strFileName.find_last_of('\\');
            assert(npos != std::string::npos);
            strFileName = strFileName.substr(npos + 1, strFileName.length() - (npos + 1));

            m_Error += strFileName + " line..";
            ss << iLine;
            m_Error += ss.str() + "    ";
        }

        m_PanicLevel++;

        std::stringstream ss;
        ss << val;
        m_Error += ss.str();

        Panic(lpcstrFileName, iLine, std::forward<Args&&>(args)...);
    }

    void Panic(const char *lpcstrFileName, int iLine)
    {
        m_PanicLevel = 0;
        throw PanicException(m_Error.c_str());
    }

private:
    int             m_PanicLevel;
    std::string     m_Error;
};
