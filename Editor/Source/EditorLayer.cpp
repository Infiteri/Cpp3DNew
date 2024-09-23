#include "EditorLayer.h"

#include <imgui.h>

namespace Core
{
    static float speed = 1.0f;
    static float rotationSpeed = 1.0f;

    static Scene *scene;

    void EditorLayer::OnAttach()
    {
        scene = World::CreateScene("Scene");
        World::ActivateScene("Scene");

        auto testActor = scene->SpawnActor();
        auto mesh = testActor->AddComponent<MeshComponent>();
    }

    void EditorLayer::OnImGuiRender()
    {
        BeginDockspace();
        PreparePanelInfo();
        state.Panels.RenderImGui(&state.PanelInfo);
        RenderSceneViewport();
        EndDockspace();

        // TODO: Move to editor layer camera class
        {
            auto cam = Core::CameraSystem::GetPerspectiveActive();
            if (cam)
            {
                cam->UpdateView();

                if (Input::GetKey(Keys::W))
                    cam->GetPosition().z -= speed;

                if (Input::GetKey(Keys::S))
                    cam->GetPosition().z += speed;

                if (Input::GetKey(Keys::A))
                    cam->GetPosition().x -= speed;

                if (Input::GetKey(Keys::D))
                    cam->GetPosition().x += speed;

                if (Input::GetKey(Keys::Q))
                    cam->GetPosition().x += speed;

                if (Input::GetKey(Keys::E))
                    cam->GetPosition().x -= speed;

                if (Input::GetKey(Keys::I))
                    cam->GetRotation().x += rotationSpeed * CE_DEG_TO_RAD;

                if (Input::GetKey(Keys::K))
                    cam->GetRotation().x -= rotationSpeed * CE_DEG_TO_RAD;

                if (Input::GetKey(Keys::J))
                    cam->GetRotation().y -= rotationSpeed * CE_DEG_TO_RAD;

                if (Input::GetKey(Keys::L))
                    cam->GetRotation().y += rotationSpeed * CE_DEG_TO_RAD;
            }
        }
    }

    void EditorLayer::OnUpdate()
    {
        World::UpdateActiveScene(); // TODO: Add this to the engine somehow please thank you :)
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
        if (viewportSize.x != state.LastFrameViewport.x || viewportSize.y != state.LastFrameViewport.y)
        {
            state.LastFrameViewport = viewportSize;
            Renderer::Viewport(viewportSize.x, viewportSize.y);
        }
        // End update renderer viewport
        ImGui::Image((void *)(CeU64)(CeU32)(Renderer::GetPassID(0)), viewportSize, ImVec2{0, 1}, ImVec2{1, 0});

        ImGui::End();
    }

    void EditorLayer::EndDockspace()
    {
        ImGui::End();
    }
}