// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>



// TODO:  在此处引用程序需要的其他头文件
#ifdef _DEBUG
#pragma message("正在编译调试版本...")
#else
#pragma message("正式编译发布版本...")
#endif // _DEBUG

#include <cassert>
#include <cstdarg>
#include <cmath>
#include <exception>
#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <stack>
#include <mutex>

#include "DikSamc.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern int  g_iCurrentThreadIndex;

#ifdef __cplusplus
}
#endif // __cplusplus
