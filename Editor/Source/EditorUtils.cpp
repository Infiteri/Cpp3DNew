#include "EditorUtils.h"
#include <imgui_internal.h>

namespace Core
{
    static CeU32 SetID = 0;

    Texture *EditorUtils::LoadTexture(const std::string &name)
    {
        auto tex = new Texture(false);
        tex->INTERNAL_REQUIRE_GENERATION = false;
        tex->Load(name, {});
        return tex;
    }

    void EditorUtils::ImGuiVec2Edit(const char *l, Vector2 *v)
    {
        float d[2] = {v->x, v->y};
        if (ImGui::DragFloat2(l, d, 0.01f))
        {
            v->Set(d[0], d[1]);
        }
    }

    void EditorUtils::ImGuiVec3Edit(const char *l, Vector3 *v)
    {
        float d[3] = {v->x, v->y, v->z};
        if (ImGui::DragFloat3(l, d, 0.01f))
        {
            v->Set(d[0], d[1], d[2]);
        }
    }

    void EditorUtils::ImGuiVector3StyledEdit(const char *label, Vector3 *vec, float defaultValue)
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

    void EditorUtils::TransformGUIRender(Transform *transform)
    {
        ImGuiVector3StyledEdit("Position", &transform->Position, 0.0f);
        ImGuiVector3StyledEdit("Rotation", &transform->Rotation, 0.0f);
        ImGuiVector3StyledEdit("Scale", &transform->Scale, 1.0f);
    }

    EditorUtils::StringEdit EditorUtils::ImGuiStringEdit(const char *label, const std::string &str)
    {
        StringEdit edit;
        const int size = 256;
        char buffer[size];
        CeMemory::Zero(&buffer, size);
        CeMemory::Copy(&buffer, str.c_str(), str.size() <= size ? str.size() : size);
        edit.Changed = ImGui::InputText(label, buffer, size);
        edit.Content = buffer;
        return edit;
    }

    std::string EditorUtils::ImGuiStringEditReturnString(const char *label, const std::string &str)
    {
        return ImGuiStringEdit(label, str).Content;
    }

    void EditorUtils::ImGuiColorEdit(const char *label, Color *color)
    {
        float data[4] = {color->r / 255, color->g / 255, color->b / 255, color->a / 255};
        if (ImGui::ColorEdit4(label, data))
        {
            color->Set(data[0] * 255, data[1] * 255, data[2] * 255, data[3] * 255);
        }
    }

    void EditorUtils::DrawTextureUI(const char *label, TextureType texType, Material *mat)
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

            // NOTE: Should be last for the drag-drop feature.
            float div = 3.0f;
            ImGui::Image((void *)(CeU64)(CeU32)texture->GetID(), {imgW / div, imgH / div});

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL"))
                {
                    const char *path = (const char *)payload->Data;
                    std::string ext = StringUtils::GetExtension(path);
                    if (ext == "jpeg" || ext == "png" || ext == "jpg")
                    {
                        switch (texType)
                        {
                        case TextureColor:
                            mat->SetColorTexture(path);
                            break;

                        default:
                            CE_WARN("No texture type defined");
                            break;
                        }
                    }
                }

                ImGui::EndDragDropTarget();
            }

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
                    CE_WARN("No texture type defined");
                    break;
                }
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Empty"))
        {

            switch (texType)
            {
            case EditorUtils::TextureColor:
                mat->SetColorTexture();
                break;

            default:
                break;
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

    bool EditorUtils::DrawDataSetUI(CeDataSet *dSet)
    {
        for (auto set : dSet->GetSet())
        {
            if (ImGui::TreeNodeEx((void *)(CeU64)(CeU32)SetID, 0, set->GetName().c_str()))
            { // todo: change
                static char Name[256];
                CeMemory::Zero(&Name, 256);
                CeMemory::Copy(&Name, set->GetName().c_str(), 256);

                if (ImGui::InputText("Name", Name, 256))
                {
                    set->SetName(Name);
                }

                // -- DATA TYPE SELECTION --
                const int maxSelections = 5;
                const char *selections[maxSelections] = {"None", "Vec2", "Vec3", "Color", "Float"};
                const char *current = selections[(int)set->GetType()];

                if (ImGui::BeginCombo("Data Type", current))
                {
                    for (int i = 0; i < maxSelections; i++)
                    {
                        bool isSelected = (current == selections[i]);

                        if (ImGui::Selectable(selections[i], isSelected))
                        {
                            current = selections[i];
                            set->ClearDataBasedOnCurrentType();
                            set->SetType((CeData::DataType)i);
                            set->SetupDefaultValuesBaseOnCurrentType();
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }
                // -------------------------

                // -- EDIT VALUES ----------
                switch (set->GetType())
                {
                case CeData::DataVec2:
                    EditorUtils::ImGuiVec2Edit("Value", set->As<Vector2>());
                    break;

                case CeData::DataVec3:
                    EditorUtils::ImGuiVec3Edit("Value", set->As<Vector3>());
                    break;

                case CeData::DataFloat:
                {
                    CeData::FloatContainer *Cont = set->As<CeData::FloatContainer>();
                    ImGui::DragFloat("Value", &Cont->Value, 0.1f);
                    break;
                }

                case CeData::DataColor:
                    EditorUtils::ImGuiColorEdit("Value", (Color *)set->As<Color>());
                    break;
                }
                // -------------------------

                if (ImGui::Button("Remove"))
                {
                    dSet->Remove(set->GetName());
                    ImGui::TreePop();
                    return true;
                }

                ImGui::TreePop();
            }

            SetID++;
        }
        return false;
    }

    void EditorUtils::SetSetIDTo0()
    {
        SetID = 0;
    }

}