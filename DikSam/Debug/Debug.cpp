#include "stdafx.h"

#include "Debug.h"

#include <assert.h>

Debug::Debug()
    : m_Line(0)
    , m_PanicLevel(0)
{

}

Debug::~Debug()
{

}

void Debug::Set(const char* lpcstrFileName, int iLine)
{
    assert(lpcstrFileName);
    assert(iLine >= 0);

    m_FileName = lpcstrFileName;
    size_t npos = m_FileName.find_last_of('\\');
    assert(npos != std::string::npos);
    m_FileName = m_FileName.substr(npos + 1, m_FileName.length() - (npos + 1));
    m_Line = iLine;
}
