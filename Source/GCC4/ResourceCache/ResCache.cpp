#include "GameCodeStd.h"
#include "ResCache.h"
#include <cctype>

#include "../Utilities/String.h"

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


ResHandle::ResHandle(Resource& resource, char* buffer, unsigned int size, ResCache* pResCache)
	: m_resource(resource)
{
	m_buffer = buffer;
	m_size = size;
	m_extra = NULL;
	m_pResCache = pResCache;
}

ResHandle::~ResHandle()
{
	SAFE_DELETE_ARRAY(m_buffer);
	m_pResCache->MemoryHasBeenFreed(m_size);
}


ResCache::ResCache(const unsigned int sizeInMb, IResourceFile* resFile)
{
	m_cacheSize = sizeInMb * 1024 * 1024;
	m_allocated = 0;
	m_file = resFile;
}

ResCache::~ResCache()
{
	while(!m_lru.empty())
	{
		FreeOneResource();
	}
	SAFE_DELETE(m_file);
}

void ResCache::RegisterLoader(shared_ptr<IResourceLoader> loader)
{
	m_resourceLoaders.push_front(loader);
}

bool ResCache::Init()
{ 
	bool retValue = false;
	if ( m_file->VOpen() )
	{
		RegisterLoader(shared_ptr<IResourceLoader>(GCC_NEW DefaultResourceLoader()));
		retValue = true;
	}
	return retValue;
}

shared_ptr<ResHandle> ResCache::GetHandle(Resource* r)
{
	shared_ptr<ResHandle> handle(Find(r));
	if(handle == NULL)
	{
		handle = Load(r);
		GCC_ASSERT(handle);
	}
	else
	{
		Update(handle);
	}
	return handle;
}


shared_ptr<ResHandle> ResCache::Load(Resource* r)
{
	shared_ptr<IResourceLoader> loader;
	shared_ptr<ResHandle> handle;
	
	for (ResourceLoaders::iterator it = m_resourceLoaders.begin(); it != m_resourceLoaders.end(); ++it)
	{
		shared_ptr<IResourceLoader> testLoader = *it;

		if (WildcardMatch(testLoader->VGetPattern().c_str(), r->m_name.c_str()))
		{
			loader = testLoader;
			break;
		}
	}

	if(!loader)
	{
		GCC_ASSERT(loader && _T("Default Resource Loader not found!"));
		return handle;
	}

	int rawSize = m_file->VGetRawResourceSize(*r);
	if(rawSize < 0)
	{
		GCC_ASSERT(rawSize > 0 && "Resource not found!");
		return shared_ptr<ResHandle>();
	}

	int allocSize = rawSize + ((loader->VAddNullZero()) ? (1) : (0));
	char* rawBuffer = loader->VUseRawFile() ? Allocate(allocSize) : GCC_NEW char[allocSize];
	memset(rawBuffer, 0, allocSize);

	if(rawBuffer == NULL || m_file->VGetRawResource(*r, rawBuffer) == 0)
	{
		//resource cache out of mem
		return shared_ptr<ResHandle>();
	}

	char *buffer = NULL;
	unsigned int size = 0;

	if(loader->VUseRawFile())
	{
		buffer = rawBuffer;
		handle = shared_ptr<ResHandle>(GCC_NEW ResHandle(*r, buffer, rawSize, this));
	}
	else
	{
		size = loader->VGetLoadedResourceSize(rawBuffer, rawSize);
		buffer = Allocate(size);
		if(rawBuffer == NULL || buffer == NULL)
		{
			//resource cache out of mem
			return shared_ptr<ResHandle>();
		}
		handle = shared_ptr<ResHandle>(GCC_NEW ResHandle(*r, buffer, rawSize, this));
		bool success = loader->VLoadResource(rawBuffer, rawSize, handle);
		if(loader->VDiscardRawBufferAfterLoad())
		{
			SAFE_DELETE_ARRAY(rawBuffer);
		}

		if(!success)
		{
			//resource cache out of mem
			return shared_ptr<ResHandle>();
		}
	}
	if(handle)
	{
		m_lru.push_front(handle);
		m_resources[r->m_name] = handle;
	}
	GCC_ASSERT(loader && _T("Default resource loader not found!"));
	return handle;		// ResCache is out of memory!
}

shared_ptr<ResHandle> ResCache::Find(Resource* r)
{
	ResHandleMap::iterator i = m_resources.find(r->m_name);
	if(i==m_resources.end())
		return shared_ptr<ResHandle>();
	return i->second;
}

void ResCache::Update(shared_ptr<ResHandle> handle)
{
	m_lru.remove(handle);
	m_lru.push_front(handle);
}

char* ResCache::Allocate(unsigned int size)
{
	if(!MakeRoom(size))
		return NULL;

	char* mem = GCC_NEW char[size];
	if(mem)
	{
		m_allocated += size;
	}

	return mem;
}