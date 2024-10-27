#pragma once

#include "Base.h"

#include "Scene/Actor.h"
#include "ActorScript.h"

#include <string>

namespace Core
{
    class CE_API ScriptEngine
    {
    public:
        ScriptEngine() {};
        ~ScriptEngine() {};

        static void Init();
        static void Shutdown();

        static void StartRuntime();
        static void StopRuntime();
        static void UpdateRuntime();
        static void ClearScriptList();

        static void RegisterScript(const std::string &scriptName, ActorScript *script, Actor *owner);
        static void RegisterScript(const std::string &name, const std::string &className, Actor *owner);
    };

}