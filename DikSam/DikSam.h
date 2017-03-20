#pragma once

#include <memory>

class Debug;

class DikSam
{
public:
    DikSam(int iThreadIndex);
    ~DikSam();

private:
    std::auto_ptr<Debug>    m_Debug;
};