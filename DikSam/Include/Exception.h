#pragma once

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

class AssertException : public std::exception
{
public:
    AssertException(const char *lpcstrWhat)
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

class MemoryException : public std::exception
{
public:
    MemoryException(const char *lpcstrWhat)
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