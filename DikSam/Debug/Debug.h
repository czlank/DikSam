#pragma once

#include "Exception.h"

class Debug
{
public:
    Debug();
    ~Debug();

    void SetLevel(int iLevel) { m_PanicLevel = iLevel; }
    void Set(const char *lpcstrFileName, int iLine);
    void SetExpression(const char *expression) { m_AssertExpression = expression; }

    template <typename T, typename ... Args> void Assert(T val, Args && ... args)
    {
        if (0 == m_PanicLevel)
        {
            std::stringstream ss;
            m_Error = "Assertion failure ";

            m_Error += m_AssertExpression + " file.." + m_FileName + " line..";
            ss << m_Line;
            m_Error += ss.str() + "    ";
        }

        std::stringstream ss;
        ss << val;
        m_Error += ss.str();

        Assert(std::forward<Args&&>(args)...);
    }

    void Assert()
    {
        m_PanicLevel = 0;
        throw AssertException(m_Error.c_str());
    }

    template <typename T, typename ... Args> void Panic(T val, Args && ... args)
    {
        if (0 == m_PanicLevel)
        {
            std::stringstream ss;
            m_Error = "Panic!! file..";
            
            m_Error += m_FileName + " line..";
            ss << m_Line;
            m_Error += ss.str() + "    ";
        }

        m_PanicLevel++;

        std::stringstream ss;
        ss << val;
        m_Error += ss.str();

        Panic(std::forward<Args&&>(args)...);
    }

    void Panic()
    {
        m_PanicLevel = 0;
        throw PanicException(m_Error.c_str());
    }

private:
    int             m_Line;
    std::string     m_FileName;
    std::string     m_AssertExpression;

    int             m_PanicLevel;
    std::string     m_Error;
};
