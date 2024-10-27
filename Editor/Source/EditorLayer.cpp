#include "EditorLayer.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Core/Utils/StringUtils.h"

#include <imgui.h>

namespace Core
{
    static Scene *scene;

    void EditorLayer::OnAttach()
    {
        ImGuiLayer::SetFont("EngineResources/Font/Open_Sans/static/OpenSans-Bold.ttf", 12.0f);

        state.Camera.camera = CameraSystem::CreatePerspective("EditorCamera", 120.0f);
        CameraSystem::Activate("EditorCamera");

        // TODO:This type of scene creation sucks
        scene = World::CreateScene("EngineResources/Scene.ce_scene");
        World::ActivateScene("EngineResources/Scene.ce_scene");

        // todo: save scene file
        CubeMapTexture::Configuration temp;
        // CubeMapLoader l;
        // l.Deserialize("EngineResources/Cubemaps/CoolBox.ce_cubemap", &temp);
        World::GetActiveScene()->GetEnvironment()->SkyInst->SetModeToColor({0, 125, 255, 255});

        // Load textures
        state.TextureSet.PlayButton = new Texture(false);
        state.TextureSet.PlayButton->INTERNAL_REQUIRE_GENERATION = false;
        state.TextureSet.PlayButton->Load("EngineResources/Images/Icons/PlayButton.png", {});

        state.TextureSet.StopButton = new Texture(false);
        state.TextureSet.StopButton->INTERNAL_REQUIRE_GENERATION = false;
        state.TextureSet.StopButton->Load("EngineResources/Images/Icons/StopButton.png", {});

        state.StateScene = SceneStateStop;

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
                        CE_TRACE("%f %f %f", matrixRotation[0], matrixRotation[1], matrixRotation[2]);
                        tc->Rotation.Set(Math::DegToRad(matrixRotation[0]), Math::DegToRad(matrixRotation[1]), Math::DegToRad(matrixRotation[2]));
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
}