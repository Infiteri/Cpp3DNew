#pragma once

#include "Core.h"
#include "Panel/PanelSystem.h"
#include "EditorCamera.h"

namespace Core
{
    struct DockspaceSettings
    {
        bool Open = true;
        bool Fullscreen = true;
        ImGuiDockNodeFlags Flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImVec2 ViewportLeftTop;
        ImVec2 ViewportRightBottom;
    };

    struct EditorState
    {
        PanelInformation PanelInfo;
        PanelSystem Panels;
        DockspaceSettings Dockspace;
        ImVec2 LastFrameViewport;
        EditorCamera Camera;
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