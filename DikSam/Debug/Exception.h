#pragma once

#include <exception>
#include <assert.h>
#include <string>

class PanicException : public std::exception
{
public:
    PanicException(const char *lpcstrWhat)
    {
        assert(lpcstrWhat);
        m_strWhat = lpcstrWhat;
    }

    virtual const char* what() const
    {
        return m_strWhat.c_str();
    }

private:
    std::string     m_strWhat;
};
