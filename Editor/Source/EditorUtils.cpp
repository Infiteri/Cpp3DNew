#include "EditorUtils.h"
#include <imgui_internal.h>

namespace Core
{
    namespace EditorUtils
    {
        void ImGuiVector3StyledEdit(const char *label, Vector3 *vec, float defaultValue)
        {
            ImGui::PushID(label);

            ImGui::Columns(2);

            ImGui::SetColumnWidth(0, 50.0f);
            ImGui::Text(label);
            ImGui::NextColumn();
            ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

            float lineH = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImVec2 buttonSize = {lineH + 3.0f, lineH};
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
            if (ImGui::Button("X", buttonSize))
            {
                vec->x = defaultValue;
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##x", &vec->x, 0.01f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
            if (ImGui::Button("Y", buttonSize))
            {
                vec->y = defaultValue;
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##y", &vec->y, 0.01f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
            if (ImGui::Button("Z", buttonSize))
            {
                vec->z = defaultValue;
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##z", &vec->z, 0.01f);
            ImGui::PopItemWidth();
            ImGui::PopStyleVar();

            ImGui::Columns(1);

            ImGui::PopID();
        }

        void TransformGUIRender(Transform *transform)
        {

            ImGuiVector3StyledEdit("Position", &transform->Position, 0.0f);
            ImGuiVector3StyledEdit("Rotation", &transform->Rotation, 0.0f);
            ImGuiVector3StyledEdit("Scale", &transform->Scale, 1.0f);
        }

        std::string ImGuiStringEdit(const char *label, const std::string &str)
        {
            const int size = 256;
            static char buffer[size];
            CeMemory::Zero(&buffer, size);
            CeMemory::Copy(&buffer, str.c_str(), str.size() <= size ? str.size() : size);
            ImGui::InputText(label, buffer, size);
            return buffer;
        }

        void ImGuiColorEdit(const char *label, Color *color)
        {
            float data[4] = {color->r / 255, color->g / 255, color->b / 255, color->a / 255};
            if (ImGui::ColorEdit4(label, data))
            {
                color->Set(data[0] * 255, data[1] * 255, data[2] * 255, data[3] * 255);
            }
        }

        void DrawTextureUI(const char *label, TextureType texType, Material *mat)
        {
            Texture *texture = nullptr;

            switch (texType)
            {
            case TextureColor:
                texture = mat->GetColorTexture();
                break;

            default:
                break;
            }

            if (texture->HasImage())
            {
                float imgW = texture->GetImage()->GetWidth();
                float imgH = texture->GetImage()->GetHeight();
                ImGui::Text("Image: %s.", texture->GetImagePath().c_str());
                ImGui::Text("Size: %f width by %f height.", imgW, imgH);

                // NOTE: Should be last for the drag-drop feature.
                float div = 5.0f;
                ImGui::Image((void *)(CeU64)(CeU32)texture->GetID(), {imgW / div, imgH / div});

                DrawTextureUIChangeFilter(texture);
            }

            if (ImGui::Button("Load"))
            {
                std::string str = Platform::OpenFileDialog("Image (*.png *.jpg *.jpeg)\0*.png;*.jpg;*.jpeg\0");
                if (!str.empty())
                {
                    switch (texType)
                    {
                    case TextureColor:
                        mat->SetColorTexture(str);
                        break;

                    default:
                        break;
                    }
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Empty"))
            {

                switch (texType)
                {
                case TextureColor:
                    mat->SetColorTexture();
                    break;

                default:
                    break;
                }
            }
        }
    }

    void EditorUtils::DrawTextureUIChangeFilter(Texture *texture)
    {
        auto &cfg = texture->GetConfig();

        for (int i = 0; i < 2; i++)
        {
            const int Count = 2;
            const char *FilterType[Count] = {"Nearest", "Linear"};
            const char *Current = FilterType[i == 0 ? (int)cfg.MinFilter : (int)cfg.MagFilter];

            if (ImGui::BeginCombo(i == 0 ? "MinFilter" : "MagFilter", Current))
            {
                for (int ci = 0; ci < Count; ci++)
                {
                    bool isSelected = (Current == FilterType[ci]);
                    if (ImGui::Selectable(FilterType[ci], isSelected))
                    {
                        Current = FilterType[ci];

                        if (i == 0)
                            cfg.MinFilter = (TextureFilter)ci;
                        else
                            cfg.MagFilter = (TextureFilter)ci;

                        texture->UpdateWithConfig(cfg);
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
        }
    }
}