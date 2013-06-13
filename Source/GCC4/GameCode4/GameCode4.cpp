#include "GameCodeStd.h"
#include "GameCode.h"
#pragma comment(lib, "tinyxml.lib")


INT WINAPI GameCode4(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	
	Logger::Init("logging.xml");

	Logger::Destroy();
    return 0;	
}

