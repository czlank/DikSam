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

class ErrorException : public std::exception
{
public:
    ErrorException(const char *lpstrWhat)
    {
        m_strWhat = lpstrWhat ? lpstrWhat : "";
    }

    ErrorException(const wchar_t *lpcwstrWhat)
    {
        assert(lpcwstrWhat);

        size_t _Dsize = sizeof(wchar_t) * std::wstring(lpcwstrWhat).length() + 1;
        char *_Dest = new char[_Dsize]();

        size_t szCount = 0;
        wcstombs_s(&szCount, _Dest, _Dsize, lpcwstrWhat, _Dsize - 1);
        _Dest[_Dsize - 1] = '\0';

        m_strWhat = _Dest;
        delete [] _Dest;
    }

    virtual const char* what() const
    {
        return m_strWhat.c_str();
    }

private:
    std::string     m_strWhat;
};