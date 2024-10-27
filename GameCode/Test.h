#pragma once

#include "Core.h"

using namespace Core;

class TestScript : public ActorScript
{
public:
    TestScript() {};
    ~TestScript() {};

    void OnStart();
    void OnUpdate();
};

CE_DEFINE_SCRIPT_EXPORT(TestScript);