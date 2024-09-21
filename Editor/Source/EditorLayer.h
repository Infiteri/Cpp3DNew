#pragma once

#include "Core.h"

namespace Core
{
    struct EditorState
    {
    };

    class EditorLayer : public Layer
    {
    public:
        EditorState state;

        EditorLayer() {};
        ~EditorLayer() {};

        void OnAttach();
        void OnImGuiRender();
        void OnUpdate();
    };
}