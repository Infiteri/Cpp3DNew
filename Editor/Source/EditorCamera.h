#pragma once

#include "Core.h"

namespace Core
{
    class EditorCamera
    {
    public:
        float speed = 0.2f;
        float rotation = 0.01f;
        float sensitivity = 0.001f;
        PerspectiveCamera *camera = nullptr;
        bool updateWithMouse = false;

        EditorCamera() = default;
        ~EditorCamera() = default;

        void UpdateMouse();
    };

}