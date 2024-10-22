#pragma once

#include "Core.h"
#include "Panel/PanelSystem.h"
#include "EditorCamera.h"

#include <ImGuizmo.h>

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
        ImGuizmo::OPERATION GuizmoOperation;
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

        void DrawGizmo(PerspectiveCamera *camera, float *data, float *deltaMatrix);
        void MapInputToGuizmoOperation(Keys k, ImGuizmo::OPERATION op);

        // -- SCENE RELATED --
        void OpenScene(const std::string &name);
        // -------------------

        // -- DOCKSPACE --
        void BeginDockspace();
        void RenderSceneViewport();
        void HandleDragDrop();
        void EndDockspace();
        // ---------------
    };
}