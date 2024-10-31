#include "SceneEnvironment.h"

namespace Core
{
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