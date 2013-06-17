#include "GameCodeStd.h"
#include "GameCode.h"

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")

#pragma comment(lib, "tinyxml.lib")
#pragma comment(lib, "zlibstat.lib")


#include "../ResourceCache/ZipFile.h"


INT WINAPI GameCode4(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	Logger::Init("Logging.xml");
	
	Logger::Destroy();
    return 0;	
}

