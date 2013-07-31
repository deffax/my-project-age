#pragma once

#include "LuaPlus.h"

class LuaStateManager : public IScriptManager
{
    static LuaStateManager* s_pSingleton;
    LuaPlus::LuaState* m_pLuaState;
    std::string m_lastError;

public:
    
    static bool Create();
    static void Destroy();
    static LuaStateManager* Get() { GCC_ASSERT(s_pSingleton); return s_pSingleton; }

    
	virtual bool VInit();
    virtual void VExecuteFile(const char* resource);
    virtual void VExecuteString(const char* str);

    LuaPlus::LuaObject GetGlobalVars();
    LuaPlus::LuaState* GetLuaState() const;

	LuaPlus::LuaObject CreatePath(const char* pathString, bool toIgnoreLastElement = false);
    

private:
    void SetError(int errorNum);
    void ClearStack();

    
    explicit LuaStateManager();
    virtual ~LuaStateManager();
};

