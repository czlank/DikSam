#pragma once

#include <memory>

#ifdef DBG_NO_DEBUG
#define DBG_assert(expression, arg)     ((void)0)
#define DBG_panic(arg)                  ((void)0)
#else
#define DBG_assert(expression, arg) ((expression) ? (void)(0) : \
    ((m_Debug.get()->Set(__FILE__, __LINE__)), (m_Debug.get()->SetExpression(#expression)), \
    m_Debug.get()->Assert arg))
#define DBG_panic(arg)  (m_Debug.get()->Set(__FILE__, __LINE__), m_Debug.get()->Panic arg)
#endif // DBG_NO_DEBUG

class Debug;

class DikSam
{
public:
    DikSam(int iThreadIndex);
    ~DikSam();

private:
    std::auto_ptr<Debug>    m_Debug;
};