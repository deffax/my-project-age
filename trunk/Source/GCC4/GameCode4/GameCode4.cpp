#include "GameCodeStd.h"
#include "GameCode.h"
//test
#include "../ResourceCache/ResCache.h"
#include <string>
using namespace std;
//end test
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")

#pragma comment(lib, "tinyxml.lib")
#pragma comment(lib, "zlibstat.lib")





INT WINAPI GameCode4(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	Logger::Init("Logging.xml");
	//test
	char* buffer = NULL;
	const basic_string <wchar_t> resFileName( L"Assets.zip" );
	ZipFile zipFile;
	if(zipFile.Init(resFileName))
	{
		optional<int> index = zipFile.Find("Logging.xml");
		if(index.valid())
		{
			int size = zipFile.GetFileLen(*index);
			buffer = new char[size];
			if(buffer)
			{
				zipFile.ReadFile(*index, buffer);
			}
		}
	}

	// end test


	Logger::Destroy();
    return 0;	
}

