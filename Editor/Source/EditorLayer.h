#pragma once

#include "Core.h"
#include "Panel/PanelSystem.h"

namespace Core
{
    struct DockspaceSettings
    {
        bool Open = true;
        bool Fullscreen = true;
        ImGuiDockNodeFlags Flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    };

    struct EditorState
    {
        PanelInformation PanelInfo;
        PanelSystem Panels;
        DockspaceSettings Dockspace;

        ImVec2 LastFrameViewport;
    };

    class EditorLayer : public Layer
    {
    public:
        EditorState state;

        EditorLayer() {};
        ~EditorLayer() {};

        // -- BASE FUNCTION --
        void OnAttach();
        void OnImGuiRender();
        void OnUpdate();
        // -------------------

        void PreparePanelInfo();

        // -- DOCKSPACE --
        void BeginDockspace();
        void RenderSceneViewport();
        void EndDockspace();
        // ---------------
    };
}