#include "EditorLayer.h"
#include "EditorUtils.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Core/Utils/StringUtils.h"

#define CE_DEFINE_COLOR_EDITABLE(name, color) state.Settings.Colors.push_back({name, color})

#include <imgui.h>

namespace Core
{
    static Scene *scene;

    void EditorLayer::OnAttach()
    {
        state.Camera.camera = CameraSystem::CreatePerspective("EditorCamera", 120.0f);
        CameraSystem::Activate("EditorCamera");

        LoadSettings();
        UseSettings();
        RegisterColorsToSettings();
        ImGuiLayer::SetFont("EngineResources/Font/Open_Sans/static/OpenSans-Bold.ttf", state.Settings.FontSize); // Fonts get set once only :

        // TODO:This type of scene creation sucks. Project soon?
        scene = World::CreateScene("EngineResources/Scene.ce_scene");
        World::ActivateScene("EngineResources/Scene.ce_scene");
        state.ActiveScenePath = scene->GetName();

        // Load textures
        state.TextureSet.PlayButton = EditorUtils::LoadTexture("EngineResources/Images/Icons/PlayButton.png");
        state.TextureSet.StopButton = EditorUtils::LoadTexture("EngineResources/Images/Icons/StopButton.png");

        StopSceneRuntime();
    }

    void EditorLayer::OnImGuiRender()
    {
        BeginDockspace();
        PreparePanelInfo();
        state.Panels.RenderImGui(&state.PanelInfo);
        RenderSceneViewport();

        // Render UI
        UI_TopBar();
        UI_MenuBar();
        UI_EditorSettings();

        // NOTE: temporary
        ImGui::Begin("Debug");
        if (ImGui::Button("Save"))
        {
            SceneSerializer serializer(scene);
            serializer.Serialize("EngineResources/Scene.ce_scene");
        }

        if (ImGui::Button("Load"))
        {
            SceneSerializer serializer(scene);
            serializer.Deserialize("EngineResources/Scene.ce_scene");
        }

        ImGui::Text("FPS: %.3f", 1.0f / Engine::GetDeltaTime());

        ImGui::End();

        EndDockspace();
    }

    void EditorLayer::OnUpdate()
    {
        switch (state.StateScene)
        {
        case SceneStatePlay:
            UpdateRuntime();
            break;

        case SceneStateStop:
            UpdateEditor();
            break;
        }
    }

    void EditorLayer::UI_TopBar()
    {
        ImGui::Begin("Top navbar", NULL, ImGuiWindowFlags_NoDecoration);

        float size = 12;

        switch (state.StateScene)
        {
        case SceneStatePlay:
            if (ImGui::ImageButton((void *)(CeU64)(CeU32)state.TextureSet.StopButton->GetID(), {size, size}))
            {
                StopSceneRuntime();
            }
            break;

        case SceneStateStop:
            if (ImGui::ImageButton((void *)(CeU64)(CeU32)state.TextureSet.PlayButton->GetID(), {size, size}))
            {
                StartSceneRuntime();
            }
            break;
        }

        ImGui::End();
    }

    void EditorLayer::UI_MenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::MenuItem("File"))
                ImGui::OpenPopup("FileMenu");

            if (ImGui::MenuItem("Editor"))
                ImGui::OpenPopup("EditorSettings");

            ImGui::SetNextWindowSize({250, 0});
            if (ImGui::BeginPopup("FileMenu"))
            {
                ImGui::SeparatorText("Scene");

                if (ImGui::MenuItem("New"))
                    NewScene();

                if (ImGui::MenuItem("Open"))
                    OpenScene();

                if (ImGui::MenuItem("Save"))
                    SaveScene();

                if (ImGui::MenuItem("Save as"))
                    SaveSceneAs();

                ImGui::EndPopup();
            }

            ImGui::SetNextWindowSize({250, 0});
            if (ImGui::BeginPopup("EditorSettings"))
            {
                if (ImGui::MenuItem("Open Editor Settings"))
                    state.RenderSettings = true;

                ImGui::EndPopup();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void EditorLayer::UI_EditorSettings()
    {
        if (!state.RenderSettings)
            return;

        float margin = 50.0f;

        ImGui::SetNextWindowPos({margin, margin});
        ImGui::SetNextWindowSize({Engine::GetWindow()->GetWidth() - (margin + 5), Engine::GetWindow()->GetHeight() - (margin + 5)});

        ImGui::Begin("Editor Settings Menu");

        ImGui::DragFloat("Camera FOV", &state.Settings.CameraFOV, 0.05f, 0.0f);
        ImGui::DragFloat("Font Size", &state.Settings.FontSize, 0.05f, 0.0f);

        ImGui::SeparatorText("Colors");

        for (auto &it : state.Settings.Colors)
            UI_UTIL_RenderColorChange(it.Name.c_str(), it.Color);

        if (ImGui::Button("Reset"))
            ImGuiLayer::SetThemeDarkDefault();

        ImGui::Dummy(ImVec2(0, 0));
        ImVec2 buttonSize = ImVec2(80, ImGui::GetFrameHeightWithSpacing());
        ImVec2 buttonPos = ImVec2(ImGui::GetWindowSize().x - buttonSize.x - ImGui::GetStyle().FramePadding.x * 2,
                                  ImGui::GetWindowSize().y - buttonSize.y - ImGui::GetStyle().FramePadding.y * 2);

        ImGui::SetCursorPos(buttonPos);

        if (ImGui::Button("Ok", buttonSize))
        {
            state.RenderSettings = false;
            SaveSettings();
            UseSettings();
        }

        ImGui::End();
    }

    void EditorLayer::UI_UTIL_RenderColorChange(const char *label, int target)
    {
        auto &colors = ImGui::GetStyle().Colors;

        float data[4] = {colors[target].x, colors[target].y, colors[target].z, colors[target].w};
        if (ImGui::ColorEdit4(label, data))
        {
            colors[target].x = data[0];
            colors[target].y = data[1];
            colors[target].z = data[2];
            colors[target].w = data[3];
        }
    }

    void EditorLayer::SaveSettings()
    {
        EditorSettingsSerializer ser(&state.Settings);
        ser.Serialize();
    }

    void EditorLayer::LoadSettings()
    {
        EditorSettingsSerializer ser(&state.Settings);
        ser.Deserialize();
    }

    void EditorLayer::RegisterColorsToSettings()
    {
        state.Settings.Colors.clear();
        CE_DEFINE_COLOR_EDITABLE("Background", ImGuiCol_WindowBg);
        CE_DEFINE_COLOR_EDITABLE("Header", ImGuiCol_Header);
        CE_DEFINE_COLOR_EDITABLE("HeaderHovered", ImGuiCol_HeaderHovered);
        CE_DEFINE_COLOR_EDITABLE("HeaderActive", ImGuiCol_HeaderActive);
        CE_DEFINE_COLOR_EDITABLE("Button", ImGuiCol_Button);
        CE_DEFINE_COLOR_EDITABLE("ButtonHovered", ImGuiCol_ButtonHovered);
        CE_DEFINE_COLOR_EDITABLE("ButtonActive", ImGuiCol_ButtonActive);
        CE_DEFINE_COLOR_EDITABLE("FrameBg", ImGuiCol_FrameBg);
        CE_DEFINE_COLOR_EDITABLE("FrameBgHovered", ImGuiCol_FrameBgHovered);
        CE_DEFINE_COLOR_EDITABLE("FrameBgActive", ImGuiCol_FrameBgActive);
        CE_DEFINE_COLOR_EDITABLE("Tab", ImGuiCol_Tab);
        CE_DEFINE_COLOR_EDITABLE("TabHovered", ImGuiCol_TabHovered);
        CE_DEFINE_COLOR_EDITABLE("TabActive", ImGuiCol_TabActive);
        CE_DEFINE_COLOR_EDITABLE("TabUnfocused", ImGuiCol_TabUnfocused);
        CE_DEFINE_COLOR_EDITABLE("TabUnfocusedActive", ImGuiCol_TabUnfocusedActive);
        CE_DEFINE_COLOR_EDITABLE("TitleBg", ImGuiCol_TitleBg);
        CE_DEFINE_COLOR_EDITABLE("TitleBgActive", ImGuiCol_TitleBgActive);
        CE_DEFINE_COLOR_EDITABLE("TitleBgCollapsed", ImGuiCol_TitleBgCollapsed);
    }

    void EditorLayer::UseSettings()
    {
        state.Camera.camera->SetFOV(state.Settings.CameraFOV);
        state.Camera.camera->UpdateProjection();
    }

    void EditorLayer::StartSceneRuntime()
    {
        state.StateScene = SceneStatePlay; // TODO: Copy scene

        World::StopActiveScene(); // Test
        state.EditorScene = Scene::From(World::GetActiveScene());
        World::StartActiveScene();
        // ActivateCamera(); NOTE: Should be done by scene start
        Renderer::Viewport();
    }

    void EditorLayer::StopSceneRuntime()
    {
        state.StateScene = SceneStateStop; // TODO: Copy scene

        if (!state.EditorScene)
            return;

        auto panel = (SceneHierarchyPanel *)state.Panels.panels[0];
        UUID id = 0;
        if (panel->selectionContext)
            id = panel->selectionContext->GetUUID();

        panel->selectionContext = nullptr;

        World::StopActiveScene(); // Stop last scene

        World::EDITOR_CopyToActive(state.EditorScene);
        delete state.EditorScene;
        state.EditorScene = nullptr;

        ActivateCamera(SceneStateStop);
        Renderer::Viewport();

        if (id != 0)
        {
            panel->selectionContext = World::GetActiveScene()->GetActorByUUID(id);
        }
    }

    void EditorLayer::ActivateCamera(CurrentSceneState mode)
    {
        switch (mode)
        {
        case SceneStatePlay:
        {
            CameraComponent *c = World::GetActiveScene()->GetPrimaryCameraComponent();
            if (!c)
                c = World::GetActiveScene()->GetFirstCameraComponent();

            CameraSystem::Activate(c->Camera);
        }
        break;

        case SceneStateStop:
        {
            CameraSystem::Activate("EditorCamera");
        }
        break;
        }
    }

    void EditorLayer::UpdateRuntime()
    {
        World::UpdateActiveScene(); // TODO: Add this to the engine somehow please thank you :)
    }

    void EditorLayer::UpdateEditor()
    {
        MapInputToGuizmoOperation(Keys::R, ImGuizmo::ROTATE);
        MapInputToGuizmoOperation(Keys::T, ImGuizmo::TRANSLATE);
        MapInputToGuizmoOperation(Keys::S, ImGuizmo::SCALE);

        if (state.Camera.updateWithMouse)
        {
            state.Camera.UpdateMouse();
            state.Camera.UpdateMovement();
        }

        if (Input::GetButton(Buttons::Right))
        {
            Vector2 position = Input::GetMousePosition();
            if (position.x > state.Dockspace.ViewportLeftTop.x &&
                position.y > state.Dockspace.ViewportLeftTop.y &&
                position.x < state.Dockspace.ViewportLeftTop.x + state.Dockspace.ViewportRightBottom.x &&
                position.y < state.Dockspace.ViewportLeftTop.y + state.Dockspace.ViewportRightBottom.y)
                state.Camera.updateWithMouse = true;
        }
        else
            state.Camera.updateWithMouse = false;
    }

    void EditorLayer::PreparePanelInfo()
    {
        // TODO: Do this :)) <3
    }

    void EditorLayer::BeginDockspace()
    {
        if (state.Dockspace.Fullscreen)
        {
            ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            state.Dockspace.WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            state.Dockspace.WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (state.Dockspace.Flags & ImGuiDockNodeFlags_PassthruCentralNode)
            state.Dockspace.WindowFlags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &state.Dockspace.Open, state.Dockspace.WindowFlags);
        ImGui::PopStyleVar();

        if (state.Dockspace.Fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle &style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 250.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), state.Dockspace.Flags);
        }

        style.WindowMinSize.x = minWinSizeX;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::PopStyleVar();
    }

    void EditorLayer::RenderSceneViewport()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
        ImGui::Begin("Viewport");
        ImGui::PopStyleVar();

        // Update renderer viewport
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        if (viewportSize.x != state.LastFrameViewport.x || viewportSize.y != state.LastFrameViewport.y)
        {
            state.LastFrameViewport = viewportSize;
            Renderer::Viewport(viewportSize.x, viewportSize.y);
        }

        state.Dockspace.ViewportLeftTop = ImGui::GetWindowPos();
        state.Dockspace.ViewportRightBottom = ImGui::GetWindowSize();

        // End update renderer viewport
        ImGui::Image((void *)(CeU64)(CeU32)(Renderer::GetPassID(0)), viewportSize, ImVec2{0, 1}, ImVec2{1, 0});

        //? Will handle all kinds of drag-n-drop from ImGui
        HandleDragDrop();

        //? Some gizmo things
        // TODO: Fix Rotation (somehow)
        {
            auto panel = (SceneHierarchyPanel *)state.Panels.panels[0];
            Actor *actorContext = panel->selectionContext;
            PerspectiveCamera *camera = CameraSystem::GetPerspectiveActive();
            if (actorContext != nullptr && camera != nullptr)
            {
                auto tc = actorContext->GetTransform();
                auto delta = actorContext->GetParent() ? Matrix4::Invert(actorContext->GetParent()->GetTransformMatrix()).data : NULL;
                auto data = actorContext->GetTransformMatrix().data;

                DrawGizmo(camera, data, delta);

                if (ImGuizmo::IsUsing())
                {
                    Matrix4 dater;
                    if (actorContext->GetParent() != nullptr)
                    {
                        Matrix4 dataMatrix = Matrix4();
                        dataMatrix.From(actorContext->GetLocalMatrix());

                        Matrix4 deltaMatrix = Matrix4();
                        if (delta)
                            deltaMatrix.From(delta);
                        else
                            deltaMatrix = Matrix4::Identity();

                        dater = Matrix4::Multiply(dataMatrix, deltaMatrix);
                    }
                    else
                    {
                        dater.From(data);
                    }

                    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
                    ImGuizmo::DecomposeMatrixToComponents(dater, matrixTranslation, matrixRotation, matrixScale);
                    // ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, dater);

                    // todo: fix
                    if (state.GuizmoOperation == ImGuizmo::TRANSLATE)
                    {
                        matrixTranslation[0] = dater[12];
                        matrixTranslation[1] = dater[13];
                        matrixTranslation[2] = dater[14];
                        tc->Position.Set(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
                    }
                    else if (state.GuizmoOperation == ImGuizmo::ROTATE)
                    {
                        Matrix4 m = dater;
                        Vector3 r = dater.GetEulerAnglesZYX();
                        tc->Rotation.Set(r);
                    }
                    else if (state.GuizmoOperation == ImGuizmo::SCALE)
                    {
                        matrixScale[0] = dater[0];
                        matrixScale[1] = dater[5];
                        matrixScale[2] = dater[10];
                        tc->Scale.Set(matrixScale[0], matrixScale[1], matrixScale[2]);
                    }
                }
            }
        }
        ImGui::End();
    }

    void EditorLayer::HandleDragDrop()
    {
        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

            if (payload)
            {
                const char *name = (const char *)payload->Data;
                std::string extension = StringUtils::GetExtension(name);

                if (extension == "ce_scene")
                    OpenScene(name);
            }

            ImGui::EndDragDropTarget();
        }
    }

    void EditorLayer::EndDockspace()
    {
        ImGui::End();
    }

    void EditorLayer::DrawGizmo(PerspectiveCamera *camera, float *data, float *deltaMatrix)
    {
        if (camera != nullptr && data != nullptr)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

            ImGuizmo::Manipulate(camera->GetViewInverted().data, camera->GetProjection().data, state.GuizmoOperation, ImGuizmo::WORLD, data, deltaMatrix);
        }
    }

    void EditorLayer::MapInputToGuizmoOperation(Keys k, ImGuizmo::OPERATION op)
    {
        if (Input::GetKey(k))
            state.GuizmoOperation = op;
    }

    void EditorLayer::OpenScene(const std::string &name)
    {
        if (!World::Exists(name))
            World::CreateScene(name);

        World::ActivateScene(name);
    }

    void EditorLayer::OpenScene()
    {
        std::string path = Platform::OpenFileDialog("*.ce_scene, \0*.ce_scene\0");
        if (!path.empty())
            OpenScene(path);
    }

    void EditorLayer::NewScene()
    {
        // TODO: Saving of current scene if present

        StopSceneRuntime();

        state.ActiveScenePath = "";

        World::CreateScene("TEMPORARY SCENE NAME");
        World::ActivateScene("TEMPORARY SCENE NAME");
    }

    void EditorLayer::SaveScene()
    {
        if (state.ActiveScenePath.empty())
        {
            SaveSceneAs();
            return;
        }

        SceneSerializer ser(World::GetActiveScene());
        ser.Serialize(state.ActiveScenePath);
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string path = Platform::SaveFileDialog("*.ce_scene \0*.ce_scene\0");
        if (!path.empty())
        {
            state.ActiveScenePath = path;
            SaveScene();
        }
    }
}