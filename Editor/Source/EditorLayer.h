#pragma once

#include "Core.h"

namespace Core
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer() {};
        ~EditorLayer() {};

        void OnAttach();
        void OnImGuiRender();
        void OnUpdate();
    };
}