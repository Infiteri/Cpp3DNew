#include "SceneSettingsPanel.h"
#include "Scene/World.h"
#include "EditorUtils.h"
#include "Platform/Platform.h"

namespace Core
{
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
                    if (ImGui::TreeNode("Shader Data"))
                    {
                        EditorUtils::DrawDataSetUI(&sky->GetShaderDataSet());

                        if (ImGui::Button("Add"))
                        {
                            sky->GetShaderDataSet().Add(new CeData(new Vector2(), CeData::DataVec2, "Data Set"));
                        }

                        ImGui::TreePop();
                    }

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

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Directional Light"))
        {
            DirectionalLight *light = &World::GetActiveScene()->GetEnvironment()->DirectionalLight;

            EditorUtils::ImGuiVec3Edit("Direction", &light->Direction);
            EditorUtils::ImGuiColorEdit("Color", &light->Color);
            ImGui::DragFloat("Intensity", &light->Intensity, 0.05f, 0.0f);

            ImGui::TreePop();
        }

        ImGui::End();
    }
}