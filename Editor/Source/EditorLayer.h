#pragma once

#include "Core.h"
#include "Panel/PanelSystem.h"
#include "EditorSetting.h"
#include "EditorCamera.h"

#include <string>
#include <vector>
#include <ImGuizmo.h>

namespace Core
{
    /// @brief Represents any scene that should be displayed in editor as opened.
    /// @note I cant explain :crying:
    struct OpenedEditorScene
    {
        OpenedEditorScene() = default;
        OpenedEditorScene(const OpenedEditorScene &other);
        std::string SceneName; // Scene filepath equivelent

        std::string ComposeDisplayName();
    };

    struct ImageViewer
    {
        ImageViewer();
        Texture *Image = nullptr;
        bool ShouldRender = false;
        float Scale = 1.0f;

        Vector2 Offset;

        void Render();
    };

    struct DockspaceSettings
    {
        bool Open = true;
        bool Fullscreen = true;
        ImGuiDockNodeFlags Flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImVec2 ViewportLeftTop;
        ImVec2 ViewportRightBottom;
    };

    struct Textures
    {
        Texture *PlayButton;
        Texture *StopButton;
    };

    enum CurrentSceneState
    {
        SceneStatePlay,
        SceneStateStop,
    };

    struct EditorState
    {
        bool RenderSettings = false;
        bool RenderEditProject = false;
        bool HasInitialProject = false;
        bool HasInitialScene = false;

        std::string ActiveProjectPath;

        PanelInformation PanelInfo;
        PanelSystem Panels;
        EditorSettings Settings;

        ImageViewer ImgViewer;
        EditorCamera Camera;
        CurrentSceneState StateScene;
        Textures TextureSet;
        Scene *EditorScene = nullptr;

        DockspaceSettings Dockspace;
        ImVec2 LastFrameViewport;
        ImGuizmo::OPERATION GuizmoOperation;

        std::vector<OpenedEditorScene> OpenedScenes;
        OpenedEditorScene ActiveOpenedScene;
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
        void ReloadLibrary();
        void Viewport();
        inline ImVec2 &GetLastFrameViewportSize() { return state.LastFrameViewport; };
        // -------------------

        // --- UI FUNCTION ---
        void UI_TopBar();
        void UI_MenuBar();
        void UI_EditorSettings();
        void UI_ProjectEdit();
        void UI_UTIL_RenderColorChange(const char *label, int target);
        // -------------------

        void SaveSettings();
        void LoadSettings();
        void RegisterColorsToSettings();
        void UseSettings();

        // -- SCENE FUNCTION --
        void StartSceneRuntime();
        void StopSceneRuntime();

        /// @param mode Play - play camera, Stop - editor camera
        void ActivateCamera(CurrentSceneState mode);

        void UpdateRuntime();
        void UpdateEditor();
        // --------------------

        void PreparePanelInfo();

        void DrawGizmo(PerspectiveCamera *camera, float *data, float *deltaMatrix);
        void MapInputToGuizmoOperation(Keys k, ImGuizmo::OPERATION op);

        // -- SCENE RELATED --
        void NewScene();
        void SaveScene();
        void SaveSceneAs();
        void OpenScene(const std::string &name);
        void OpenScene();

        void OpenEditorScene(const std::string &name);
        // -------------------

        // -- PROJECT RELATED --
        void NewProject();
        void SaveProject();
        void OpenProject();
        void OpenProject(const std::string &name);
        void SetContextToProject();
        // ---------------------

        // -- DOCKSPACE --
        void BeginDockspace();
        void RenderSceneViewport();
        void HandleDragDrop();
        void EndDockspace();
        // ---------------

        static EditorLayer *GetInstance();
    };
}