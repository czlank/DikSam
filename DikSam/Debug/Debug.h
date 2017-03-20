#pragma once

#include "Exception.h"

#include <string>
#include <sstream>

#ifdef DBG_NO_DEBUG
#define DBG_panic(arg)      ((void)0)
#else
#define DBG_panic(arg)  (m_Debug.get()->Set(__FILE__, __LINE__), m_Debug.get()->Panic arg)
#endif // DBG_NO_DEBUG


class Debug
{
public:
    Debug();
    ~Debug();

    void Set(const char* lpcstrFileName, int iLine);

    template<typename T, typename ... Args> void Panic(T val, Args && ... args)
    {
        if (0 == m_PanicLevel)
        {
            std::stringstream ss;
            m_strPanic = "Panic!! file..";
            
            m_strPanic += m_FileName + " line..";
            ss << m_Line;
            m_strPanic += ss.str() + " ";
        }

        m_PanicLevel++;

        std::stringstream ss;
        ss << val;
        m_strPanic += ss.str();

        Panic(std::forward<Args&&>(args)...);
    }

    void Panic()
    {
        m_PanicLevel = 0;
        throw PanicException(m_strPanic.c_str());
    }

private:
    int             m_Line;
    std::string     m_FileName;
    std::string     m_AssertExpression;

    int             m_PanicLevel;
    std::string     m_strPanic;
};
