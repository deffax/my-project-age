#include "GameCodeStd.h"
#include "ResCache.h"
#include <cctype>

Resource::Resource(const std::string& name)
{
	m_name = name;
	std::transform(m_name.begin(), m_name.end(), m_name.begin(),
		(int(*)(int)) std::tolower);
}

ResourceZipFile::~ResourceZipFile() 
{ 
	SAFE_DELETE(m_pZipFile); 
}


bool ResourceZipFile::VOpen()
{
	m_pZipFile = GCC_NEW ZipFile;
    if (m_pZipFile)
    {
		return m_pZipFile->Init(m_resFileName.c_str());
	}
	return false;	
}



int ResourceZipFile::VGetRawResourceSize(const Resource &r)
{
	int resourceNum = m_pZipFile->Find(r.m_name.c_str());
	if (resourceNum == -1)
		return -1;

	return m_pZipFile->GetFileLen(resourceNum);
}



int ResourceZipFile::VGetRawResource(const Resource &r, char *buffer)
{
	int size = 0;
	optional<int> resourceNum = m_pZipFile->Find(r.m_name.c_str());
	if (resourceNum.valid())
	{
		size = m_pZipFile->GetFileLen(*resourceNum);
		m_pZipFile->ReadFile(*resourceNum, buffer);
	}
	return size;	
}



int ResourceZipFile::VGetNumResources() const 
{ 
	return (m_pZipFile==NULL) ? 0 : m_pZipFile->GetNumFiles(); 
}




std::string ResourceZipFile::VGetResourceName(int num) const 
{ 
	std::string resName = "";
	if (m_pZipFile!=NULL && num>=0 && num<m_pZipFile->GetNumFiles())
	{
		resName = m_pZipFile->GetFilename(num); 
	}
	return resName;
}