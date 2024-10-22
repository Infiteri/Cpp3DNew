#include "SceneSettingsPanel.h"
#include "Scene/World.h"
#include "EditorUtils.h"
#include "Platform/Platform.h"

namespace Core
{
    static float intensity = 0.5;
    static float blurRadius = 0.5;
    static float blurStrength = 0.5;
    static int samples = 1;

    SceneSettingsPanel::SceneSettingsPanel()
    {
    }

    SceneSettingsPanel::~SceneSettingsPanel()
    {
    }

    void SceneSettingsPanel::OnImGuiRender(PanelInformation *info)
    {
        ImGui::Begin("Scene Settings");

        if (ImGui::TreeNode("Sky"))
        {
            auto sky = World::GetActiveScene()->GetEnvironment()->SkyInst;

            if (sky)
            {
                const int maxSelections = 3;
                const char *selections[maxSelections] = {"Color", "CubeMap", "Shader"};
                const char *current = selections[(int)sky->GetMode()];

                if (ImGui::BeginCombo("Data Type", current))
                {
                    for (int i = 0; i < maxSelections; i++)
                    {
                        bool isSelected = (current == selections[i]);

                        if (ImGui::Selectable(selections[i], isSelected))
                        {
                            current = selections[i];
                            sky->SetMode((Sky::Mode)i);
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                switch (sky->GetMode())
                {
                case Sky::ColorMode:
                default:
                    EditorUtils::ImGuiColorEdit("Color", &sky->GetColor());
                    break;

                case Sky::CubeMapMode:
                    if (ImGui::Button("Load"))
                    {
                        std::string path = Platform::OpenFileDialog("Cubemap.ce_cubemap \0*.ce_cubemap\0");
                        if (!path.empty())
                            sky->SetModeToCubeMap(path);
                    }

                    if (ImGui::Button("ReLoad"))
                    {
                        sky->SetModeToCubeMap(sky->GetCubemapPath());
                    }
                    break;

                case Sky::ShaderMode:
                    if (ImGui::Button("Load"))
                    {
                        std::string path = Platform::OpenFileDialog("GLSL Shader \0*.glsl\0");
                        if (!path.empty())
                            sky->SetModeToShader(path);
                    }

                    if (ImGui::Button("ReLoad"))
                    {
                        sky->SetModeToShader(sky->GetShaderName());
                    }
                    break;
                }
            }

            EditorUtils::DrawDataSetUI(&sky->GetShaderDataSet());
            ImGui::TreePop();
        }

        ImGui::DragFloat("Intensity", &intensity, 0.005f);
        ImGui::DragFloat("Blur Radius", &blurRadius, 0.005f);
        ImGui::DragFloat("Blur Strength", &blurStrength, 0.005f);
        ImGui::DragInt("Samples", &samples);

        Renderer::TEMP_GetShaderFromPost(0)->Float(intensity, "intensity");
        Renderer::TEMP_GetShaderFromPost(0)->Float(blurRadius, "blurRadius");
        Renderer::TEMP_GetShaderFromPost(0)->Float(blurStrength, "blurStrength");
        Renderer::TEMP_GetShaderFromPost(0)->Int(samples, "samples");

        ImGui::End();
    }
}