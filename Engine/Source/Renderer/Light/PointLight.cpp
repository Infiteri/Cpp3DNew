#include "PointLight.h"
#include "Core/Logger.h"
#include "Renderer/Shader/ShaderSystem.h"

#include <string>

namespace Core
{
    PointLight::PointLight()
    {
    }

    PointLight::~PointLight()
    {
    }

    void PointLight::Render()
    {
        auto id = LightID::GetNewPointLight();
        auto shd = ShaderSystem::GetFromEngineResource("Object");

        if (!shd)
            return;
        std::string fmt = "pointLights[" + std::to_string(id) + "]";

        shd->Vec3(Position, (fmt + ".position").c_str());
        shd->Vec4(Color, (fmt + ".color").c_str());
        shd->Vec3(Specular, (fmt + ".specular").c_str());
        shd->Float(Constant, (fmt + ".constant").c_str());
        shd->Float(Linear, (fmt + ".linear").c_str());
        shd->Float(Quadratic, (fmt + ".quadratic").c_str());
        shd->Float(Radius, (fmt + ".radius").c_str());
        shd->Float(Intensity, (fmt + ".intensity").c_str());
    }
}