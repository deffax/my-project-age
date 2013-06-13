#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


//#include "targetver.h"
#define NOMINMAX
#include <windows.h>
#include <string>
#include <map>
#include <list>

#include <tinyxml.h>

#include "../Debugging/Logger.h"



#if defined(_DEBUG)
#   define GCC_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#   define GCC_NEW new
#endif

#pragma warning(disable : 4996)

class GCC_noncopyable
{
private:
	GCC_noncopyable(const GCC_noncopyable& x);
	GCC_noncopyable& operator=(const GCC_noncopyable& x);
public:
	GCC_noncopyable() {};
};

extern INT WINAPI GameCode4(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow);

