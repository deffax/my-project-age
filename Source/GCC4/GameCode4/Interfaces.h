#pragma once

class Resource;
class IResourceFile;
class ResHandle;

class IResourceFile
{
public:
	virtual bool VOpen() = 0;
	virtual int VGetRawResourceSize(const Resource &r) = 0;
	virtual int VGetRawResource(const Resource &r, char* buffer) = 0;
	virtual int VGetNumResource() const = 0;
	virtual std::string VGetResourceName(int num) const = 0;
	virtual bool VIsUsingDevelopmentDirectories() const = 0;
	virtual ~IResourceFile(){}
};

