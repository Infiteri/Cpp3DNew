#pragma once

#include "Base.h"
#include "Camera.h"
#include "PerspectiveCamera.h"

#include <string>

namespace Core
{
    class CE_API CameraSystem
    {
    public:
        CameraSystem(){};
        ~CameraSystem(){};

        static void Init();
        static void Shutdown();

        static PerspectiveCamera *CreatePerspective(const std::string &name, float fov = 90, float aspect = 1280 / 720, float near = 0.01f, float far = 1000.0f);

        static Camera *GetCamera(const std::string &name);
        static PerspectiveCamera *GetPerspective(const std::string &name);

        static void Activate(const std::string &name);
        static void Activate(Camera *camera);

        static Camera *GetActive();
        static PerspectiveCamera *GetPerspectiveActive();
    };

}
