#include "ScriptEngine.h"
#include "Platform/Platform.h"

#include <unordered_map>

namespace Core
{
    static Platform::DynamicLibrary gameCodeLibrary;
    static std::unordered_map<std::string, ActorScript *> scripts;

    typedef ActorScript *(*GetScriptPFN)();

    void ScriptEngine::Init()
    {
        gameCodeLibrary = Platform::CreateLibrary("GameCode.dll"); // TODO: project
    }

    void ScriptEngine::Shutdown()
    {
        ClearScriptList();
    }

    void ScriptEngine::StartRuntime()
    {
        for (auto it : scripts)
            it.second->OnStart();
    }

    void ScriptEngine::StopRuntime()
    {
        for (auto it : scripts)
            it.second->OnStop();

        ClearScriptList();
    }

    void ScriptEngine::UpdateRuntime()
    {
        for (auto it : scripts)
            it.second->OnUpdate();
    }

    void ScriptEngine::ClearScriptList()
    {
        for (auto it : scripts)
            delete it.second;

        scripts.clear();
    }

    void ScriptEngine::RegisterScript(const std::string &scriptName, ActorScript *script, Actor *owner)
    {
        if (scripts.find(scriptName) != scripts.end())
            return;

        script->Owner = owner;
        scripts[scriptName] = script;
    }

    void ScriptEngine::RegisterScript(const std::string &name, const std::string &className, Actor *owner)
    {
        if (scripts.find(name) != scripts.end())
            return;

        auto func = Platform::GetFunction<GetScriptPFN>(&gameCodeLibrary, className + "Create");
        if (func)
            RegisterScript(name, func(), owner);
    }
}