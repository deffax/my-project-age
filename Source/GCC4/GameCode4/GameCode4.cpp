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
	/*char* buffer = NULL;
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
	}*/
	const basic_string <wchar_t> resFileName( L"Assets.zip" );
	ResourceZipFile* zipFile = GCC_NEW ResourceZipFile(resFileName);
	//ResCache resCache(50, zipFile);
	ResCache* resCache = GCC_NEW ResCache(50, zipFile);
	if(resCache->Init())
	{
		Resource resource("\\Logger.xml");
		shared_ptr<ResHandle> handle = resCache->GetHandle(&resource);
		int size = handle->Size();
		char *xfile = (char*) handle->Buffer();
	}
	// end test


	Logger::Destroy();
    return 0;	
}

