#include "SceneEnvironment.h"

namespace Core
{
    SceneEnvironment::~SceneEnvironment()
    {
        CE_VERIFY(SkyInst);
        delete SkyInst;
        SkyInst = nullptr;
    }

    void SceneEnvironment::Setup()
    {
        SkyInst = new Sky();
    }

    void SceneEnvironment::From(SceneEnvironment *env)
    {
        if (!env || !env->SkyInst)
            return;

        SkyInst->From(env->SkyInst);

        DirectionalLight.Color = env->DirectionalLight.Color;
        DirectionalLight.Direction = env->DirectionalLight.Direction;
        DirectionalLight.Intensity = env->DirectionalLight.Intensity;
    }
}