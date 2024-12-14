#pragma once

#include "Base.h"

#include "Scene/Actor.h"
#include "ActorScript.h"

#include <string>

namespace Core
{
    /// @note Anything 'ScriptLibrary' related is referring to the base script library, the one with all the actor scripts and alike.
    class CE_API ScriptEngine
    {
    public:
        ScriptEngine() {};
        ~ScriptEngine() {};

        static void Init(const std::string &scriptLibrary = "");
        static void Shutdown();

        static void StartRuntime();
        static void StopRuntime();
        static void UpdateRuntime();
        static void ClearScriptList();

        static void RegisterScript(const std::string &scriptName, ActorScript *script, Actor *owner);
        static void RegisterScript(const std::string &name, const std::string &className, Actor *owner);

        static void UnloadScriptLibrary();
        static void LoadScriptLibrary(const std::string &name);
        static void ReloadScriptLibrary(const std::string &name);
    };

}