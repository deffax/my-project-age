#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


//#include "targetver.h"
#define NOMINMAX
#include <windows.h>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <memory.h>


// DirectX Includes

#include <d3dx9tex.h>
#include <dxut.h>
#include <SDKmisc.h>
#include <tinyxml.h>

#include <tchar.h>

#include <algorithm>

#include "../Debugging/Logger.h"
#include "../Utilities/templates.h"
#include "../Utilities/types.h"

using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;

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

#include "../GameCode4/Interfaces.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern const int MEGABYTE;
extern const float SIXTY_HERTZ;

#if !defined(SAFE_DELETE)
	#define SAFE_DELETE(x) if(x) delete x; x=NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
	#define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=NULL; 
#endif

#if !defined(SAFE_RELEASE)
	#define SAFE_RELEASE(x) if(x) x->Release(); x=NULL;
#endif


#include "../GameCode4/GameCode.h"
extern INT WINAPI GameCode4(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow);

