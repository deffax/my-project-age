#pragma once


class Actor;
class ActorComponent;

typedef unsigned int ActorId;
typedef unsigned int ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef shared_ptr<Actor> StrongActorPtr;
typedef shared_ptr<ActorComponent> StrongActorComponentPtr;
typedef weak_ptr<Actor> WeakActorPtr;
typedef weak_ptr<ActorComponent> WeakActorComponentPtr;


class Resource;
class IResourceFile;
class ResHandle;


class IResourceLoader
{
public:
	virtual std::string VGetPattern()=0;
	virtual bool VUseRawFile()=0;
	virtual bool VDiscardRawBufferAfterLoad()=0;
    virtual bool VAddNullZero() { return false; }
	virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)=0;
	virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle)=0;
};

class IResourceFile
{
public:
	virtual ~IResourceFile() { }
	virtual bool VOpen()=0;
	virtual int VGetRawResourceSize(const Resource &r)=0;
	virtual int VGetRawResource(const Resource &r, char *buffer)=0;
	virtual int VGetNumResources() const = 0;
	virtual std::string VGetResourceName(int num) const = 0;
    virtual bool VIsUsingDevelopmentDirectories(void) const = 0;	
};