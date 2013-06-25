#pragma once
#include "ZipFile.h"

class Resource
{
public:
	std::string m_name;
	Resource(const std::string& name);
};

class ResourceZipFile : public IResourceFile
{
	ZipFile* m_pZipFile;
	std::wstring m_resFileName;
public:
	ResourceZipFile(const std::wstring resFileName)
	{
		m_pZipFile = NULL;
		m_resFileName = resFileName;
	}

	virtual ~ResourceZipFile();
	virtual bool VOpen();
	virtual int VGetRawResourceSize(const Resource& r);
	virtual int VGetRawResource(const Resource& r, char* buffer);
	virtual int VGetNumResources() const;
	virtual std::string VGetResourceName(int num) const;
	virtual bool VIsUsingDevelopmenteDirectory() const
	{
		return false;
	}
};