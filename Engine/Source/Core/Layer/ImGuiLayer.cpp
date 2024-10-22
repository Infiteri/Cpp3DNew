#include "ImGuiLayer.h"

#include "Core/Engine.h"
#include "Core/Window.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <ImGuizmo.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Core
{
    void ImGuiLayer::Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        // TODO: Check if path is valid

        ImGui::StyleColorsDark();

        // Will be started by the application, after the Engine::SetupWithApplication call
        ImGui_ImplGlfw_InitForOpenGL(Engine::GetWindow()->GetHandle(), true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        // TODO: Make CONFIGURABLE and SAVABLE
        auto &colors = ImGui::GetStyle().Colors;

        colors[ImGuiCol_WindowBg] = ImVec4{0.13f, 0.14f, 0.16f, 1.0f};

        // Headers
        colors[ImGuiCol_Header] = ImVec4{0.2f, 0.25f, 0.3f, 1.0f};
        colors[ImGuiCol_HeaderHovered] = ImVec4{0.25f, 0.3f, 0.35f, 1.0f};
        colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.2f, 0.25f, 1.0f};

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{0.2f, 0.25f, 0.3f, 1.0f};
        colors[ImGuiCol_ButtonHovered] = ImVec4{0.25f, 0.3f, 0.35f, 1.0f};
        colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.2f, 0.25f, 1.0f};

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.25f, 0.3f, 1.0f};
        colors[ImGuiCol_FrameBgHovered] = ImVec4{0.25f, 0.3f, 0.35f, 1.0f};
        colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.2f, 0.25f, 1.0f};

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.2f, 0.25f, 1.0f};
        colors[ImGuiCol_TabHovered] = ImVec4{0.35f, 0.4f, 0.45f, 1.0f};
        colors[ImGuiCol_TabActive] = ImVec4{0.25f, 0.3f, 0.35f, 1.0f};
        colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.2f, 0.25f, 1.0f};
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.25f, 0.3f, 1.0f};

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.2f, 0.25f, 1.0f};
        colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.2f, 0.25f, 1.0f};
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.2f, 0.25f, 1.0f};
    }

    void ImGuiLayer::Shutdown()
    {
    }

    void ImGuiLayer::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::EndFrame()
    {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(Engine::GetWindow()->GetWidth(), Engine::GetWindow()->GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup);
        }
    }

    void ImGuiLayer::SetFont(const char *path, float size)
    {
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.Fonts->AddFontFromFileTTF(path, size);
    }
}