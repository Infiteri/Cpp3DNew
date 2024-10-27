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
    }

}