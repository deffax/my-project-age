#include "GameCodeStd.h"
#include "LuaStateManager.h"
#include "../Utilities/String.h"

#pragma comment(lib, "luaplus51-1201.lib")

LuaStateManager* LuaStateManager::s_pSingleton = NULL;

bool LuaStateManager::Create()
{
    if (s_pSingleton)
    {
        GCC_ERROR("Overwriting LuaStateManager singleton");
        SAFE_DELETE(s_pSingleton);
    }

    s_pSingleton = GCC_NEW LuaStateManager;
    if (s_pSingleton)
        return s_pSingleton->VInit();

    return false;
}

void LuaStateManager::Destroy()
{
    GCC_ASSERT(s_pSingleton);
    SAFE_DELETE(s_pSingleton);
}

LuaStateManager::LuaStateManager()
{
    m_pLuaState = NULL;
}

LuaStateManager::~LuaStateManager()
{
    if (m_pLuaState)
    {
        LuaPlus::LuaState::Destroy(m_pLuaState);
        m_pLuaState = NULL;
    }
}

bool LuaStateManager::VInit()
{
    m_pLuaState = LuaPlus::LuaState::Create();
    if (m_pLuaState == NULL)
        return false;

    
    m_pLuaState->GetGlobals().RegisterDirect("ExecuteFile", (*this), &LuaStateManager::VExecuteFile);
    m_pLuaState->GetGlobals().RegisterDirect("ExecuteString", (*this), &LuaStateManager::VExecuteString);

    return true;
}

void LuaStateManager::VExecuteFile(const char* path)
{
    int result = m_pLuaState->DoFile(path);
    if (result != 0)
        SetError(result);
}

void LuaStateManager::VExecuteString(const char* chunk)
{
    int result = 0;

    
    if (strlen(chunk) <= 1 || chunk[0] != '=')
    {
        result = m_pLuaState->DoString(chunk);
        if (result != 0)
            SetError(result);
    }

   
    else
    {
        std::string buffer("print(");
        buffer += (chunk + 1);
        buffer += ")";
        result = m_pLuaState->DoString(buffer.c_str());
        if (result != 0)
            SetError(result);
    }
}

void LuaStateManager::SetError(int errorNum)
{
   

    LuaPlus::LuaStackObject stackObj(m_pLuaState,-1);
    const char* errStr = stackObj.GetString();
    if (errStr)
    {
        m_lastError = errStr;
        ClearStack();
    }
    else
        m_lastError = "Unknown Lua parse error";

    GCC_ERROR(m_lastError);
}

void LuaStateManager::ClearStack()
{
    m_pLuaState->SetTop(0);
}

LuaPlus::LuaObject LuaStateManager::GetGlobalVars()
{
    GCC_ASSERT(m_pLuaState);
    return m_pLuaState->GetGlobals();
}

LuaPlus::LuaState* LuaStateManager::GetLuaState() const
{
    return m_pLuaState;
}

LuaPlus::LuaObject LuaStateManager::CreatePath(const char* pathString, bool toIgnoreLastElement)
{
    StringVec splitPath;
    Split(pathString, splitPath, '.');
    if (toIgnoreLastElement)
        splitPath.pop_back();

    LuaPlus::LuaObject context = GetGlobalVars();
    for (auto it = splitPath.begin(); it != splitPath.end(); ++it)
    {
        
        if (context.IsNil())
        {
            GCC_ERROR("Something broke in CreatePath(); bailing out (element == " + (*it) + ")");
            return context;  
        }

        
        const std::string& element = (*it);
        LuaPlus::LuaObject curr = context.GetByName(element.c_str());

        if (!curr.IsTable())
        {
            
            if (!curr.IsNil())
            {
                GCC_WARNING("Overwriting element '" + element + "' in table");
                context.SetNil(element.c_str());
            }

           
            context.CreateTable(element.c_str());
        }

        context = context.GetByName(element.c_str());
    }

    
    return context;
}