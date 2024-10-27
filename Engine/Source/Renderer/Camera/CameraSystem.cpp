#include "CameraSystem.h"
#include "Core/Logger.h"
#include <unordered_map>

namespace Core
{
    static std::unordered_map<std::string, Camera *> cameras;
    static Camera *active;

    void CameraSystem::Init()
    {
        active = nullptr;
    }

    void CameraSystem::Shutdown()
    {
        for (auto c : cameras)
            delete c.second;
        cameras.clear();
    }

    PerspectiveCamera *CameraSystem::CreatePerspective(const std::string &name, float fov, float aspect, float near, float far)
    {
        PerspectiveCamera *c = new PerspectiveCamera(fov, aspect, near, far);
        cameras[name] = c;
        return c;
    }

    Camera *CameraSystem::GetCamera(const std::string &name)
    {
        return cameras.find(name)->second;
    }

    PerspectiveCamera *CameraSystem::GetPerspective(const std::string &name)
    {
        return (PerspectiveCamera *)GetCamera(name);
    }

    void CameraSystem::Activate(const std::string &name)
    {
        if (!GetCamera(name))
        {
            CE_LOG("RENDER", Error, "Camera: %s non-existing when calling 'Activate'.", name.c_str());
            return;
        }

        active = GetCamera(name);
        if (!active)
        {
            CE_LOG("RENDER", Error, "Camera: %s non-existing when calling 'Activate', no camera got.", name.c_str());
            return;
        }

        active->UpdateProjection();
        active->UpdateView();
    }

    void CameraSystem::Activate(Camera *camera)
    {
        active = camera;
        active->UpdateProjection();
        active->UpdateView();
    }

    Camera *CameraSystem::GetActive()
    {
        return active;
    }

    PerspectiveCamera *CameraSystem::GetPerspectiveActive()
    {
        return (PerspectiveCamera *)active;
    }
}