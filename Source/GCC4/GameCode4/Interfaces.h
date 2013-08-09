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

template<class T>
struct SortBy_SharedPtr_Content
{
    bool operator()(const shared_ptr<T> &lhs, const shared_ptr<T> &rhs) const
        { return *lhs < *rhs; }
};



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
	virtual ~IResourceFile() {}
	virtual bool VOpen()=0;
	virtual int VGetRawResourceSize(const Resource &r)=0;
	virtual int VGetRawResource(const Resource &r, char *buffer)=0;
	virtual int VGetNumResources() const = 0;
	virtual std::string VGetResourceName(int num) const = 0;
    virtual bool VIsUsingDevelopmentDirectories(void) const = 0;	
};

class IScriptManager
{
public:
    virtual ~IScriptManager() {}
    virtual bool VInit() = 0;
    virtual void VExecuteFile(const char* resource) = 0;
    virtual void VExecuteString(const char* str) = 0;
};


class IScreenElement
{
public:
	virtual HRESULT VOnRestore() = 0;
	virtual HRESULT VOnLostDevice() = 0;
	virtual HRESULT VOnRender(double ftime, float fElapsedTime) = 0;
	virtual void VOnUpdate(int deltaMilliseconds) = 0;
	virtual int VGetZOrder() const = 0;
	virtual void VSetZOrder(int const ZOrder) = 0;
	virtual bool VIsVisible() const = 0;
	virtual bool VSetVisible(bool visible) = 0;
	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;
	virtual ~IScreenElement() {}
	virtual bool const operator<(IScreenElement const &other)
	{
		return VGetZOrder() < other.VGetZOrder();
	}
};

typedef std::list<shared_ptr<IScreenElement> > ScreenElementList;

class IRenderer
{
public:
	virtual void VSetBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB) = 0;
	virtual HRESULT VOnRestore() = 0;
	virtual void VShutDown() = 0;
	virtual bool VPreRender() = 0;
	virtual bool VPostRender() = 0;
	virtual void VDrawLine(const Vec3& from, const Vec3& to, const Color& color) = 0;
};

enum GameViewType
{
	GameView_Human,
	GameView_Remote,
	GameView_AI,
	GameView_Recorder,
	GameView_Other
};

typedef unsigned int GameViewId;
extern const GameViewId gc_InvalidGameViewId;

class IGameView
{
public:
	virtual void VOnRestore() = 0;
	virtual void VOnRender(double fTime, float elapsedTime) = 0;
	virtual void VOnLostDevice() = 0;
	virtual GameViewType VGetType() const = 0;
	virtual GameViewId VGetId() const = 0;
	virtual void VOnAttach(GameViewId vid, ActorId aid) = 0;
	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;
	virtual void VOnUpdate(unsigned long deltaMs) = 0;
	virtual ~IGameView() {};
};

class IKeyboardHandler
{
public:
	virtual bool VOnKeyDown(const BYTE c)=0;
	virtual bool VOnKeyUp(const BYTE c)=0;
};

class IPointerHandler
{
public:
	virtual bool VOnPointerMove(const Point &pos, const int radius) = 0;
	virtual bool VOnPointerButtonDown(const Point &pos, const int radius, const std::string &buttonName) = 0;
	virtual bool VOnPointerButtonUp(const Point &pos, const int radius, const std::string &buttonName) = 0;
};

class IDummy
{
	virtual void met1() = 0;
	virtual void met2() = 0;
};

typedef std::list<shared_ptr<IGameView> > GameViewList;

class IGameLogic
{
public:
	/*virtual WeakActorPtr VGetActor(const ActorId id) = 0;
	virtual StrongActorPtr VCreateActor(const std::string& actorResource, TiXmlElement* overrides,
		Mat4x4* initialTransform = NULL) = 0;*/
	//virtual void VDestroyActor(const ActorId actorId) = 0;
	virtual void VOnUpdate(float time, float elapsedTime) = 0;
	virtual void VChangeState(enum BaseGameState newState) = 0;

};