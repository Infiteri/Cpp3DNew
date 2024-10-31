#include "DirectionalLight.h"
#include "Renderer/Shader/ShaderSystem.h"

namespace Core
{
    DirectionalLight::DirectionalLight()
    {
    }

    DirectionalLight::~DirectionalLight()
    {
    }

    void DirectionalLight::Render()
    {
        auto shd = ShaderSystem::GetFromEngineResource("Object");

        if (!shd)
            return;

        shd->Float(Intensity, "directionalLight.intensity");
        shd->Vec4(Color, "directionalLight.color");
        shd->Vec3(Direction, "directionalLight.direction");
    }
}