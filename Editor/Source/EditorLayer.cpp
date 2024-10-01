#include "EditorLayer.h"

#include <imgui.h>

namespace Core
{
    static float speed = 1.0f;
    static float rotationSpeed = 1.0f;

    static Scene *scene;

    void EditorLayer::OnAttach()
    {
        state.Camera.camera = CameraSystem::CreatePerspective("EditorCamera", 120.0f);
        CameraSystem::Activate("EditorCamera");

        ImGuiLayer::SetFont("EngineResources/Font/Open_Sans/static/OpenSans-Bold.ttf", 12.0f);

        scene = World::CreateScene("Scene");
        World::ActivateScene("Scene");

        auto testActor = scene->SpawnActor();
        auto mesh = testActor->AddComponent<MeshComponent>();

        auto testActor2 = testActor->SpawnChild();
        testActor2->SetName("Child");
        auto mesh2 = testActor2->AddComponent<MeshComponent>();

        auto testActor3 = testActor2->SpawnChild();
        testActor3->SetName("Child");
        auto mesh3 = testActor3->AddComponent<MeshComponent>();

        testActor2->GetTransform()->Position = {6, 1, 0};
        testActor2->GetTransform()->Rotation = {0, 90 * CE_DEG_TO_RAD, 0};
        testActor3->GetTransform()->Position = {5, 0, 0};
    }

    void EditorLayer::OnImGuiRender()
    {
        BeginDockspace();
        PreparePanelInfo();
        state.Panels.RenderImGui(&state.PanelInfo);
        RenderSceneViewport();
        EndDockspace();
    }

    void EditorLayer::OnUpdate()
    {
        World::UpdateActiveScene(); // TODO: Add this to the engine somehow please thank you :)

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
        {
            state.Camera.updateWithMouse = false;
        }
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
        ImGui::Begin("Viewport");

        // Update renderer viewport
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        state.LastFrameViewport = viewportSize;
        if (viewportSize.x != state.LastFrameViewport.x || viewportSize.y != state.LastFrameViewport.y)
        {
            Renderer::Viewport(viewportSize.x, viewportSize.y);
        }

        state.Dockspace.ViewportLeftTop = ImGui::GetWindowPos();
        state.Dockspace.ViewportRightBottom = ImGui::GetWindowSize();

        // End update renderer viewport
        ImGui::Image((void *)(CeU64)(CeU32)(Renderer::GetPassID(0)), viewportSize, ImVec2{0, 1}, ImVec2{1, 0});

        ImGui::End();
    }

    void EditorLayer::EndDockspace()
    {
        ImGui::End();
    }
}