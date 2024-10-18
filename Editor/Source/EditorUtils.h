#pragma once

#include "Core.h"
#include "Core/Data/CeDataSet.h"

#include <imgui.h>
#include <string>

namespace Core
{
    namespace EditorUtils
    {
        enum TextureType
        {
            TextureColor,
        };

        struct StringEdit
        {
            std::string Content;
            bool Changed;
        };

        void ImGuiVec2Edit(const char *l, Vector2 *v);
        void ImGuiVec3Edit(const char *l, Vector3 *v);

        void ImGuiVector3StyledEdit(const char *label, Vector3 *vec, float defaultValue = 0.0f);
        void TransformGUIRender(Transform *transform);
        StringEdit ImGuiStringEdit(const char *label, const std::string &str);
        void ImGuiColorEdit(const char *label, Color *color);
        void DrawTextureUI(const char *label, TextureType texType, Material *mat);
        void DrawTextureUIChangeFilter(Texture *texture);

        bool DrawDataSetUI(CeDataSet *set);
        void SetSetIDTo0();

        template <typename T, typename UIFun>
        void DrawComponentBaseUI(const std::string &name, T *component, int index, Actor *a, UIFun fun)
        {
            const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap;
            if (component)
            {

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4, 4});
                bool open = ImGui::TreeNodeEx((void *)(typeid(T).hash_code() + index), flags, name.c_str());
                bool removeComp = false;

                ImGui::PopStyleVar();

                if (open)
                {
                    fun(component, a);
                    if (ImGui::Button("Remove"))
                    {
                        removeComp = true;
                    };
                    ImGui::TreePop();
                }

                if (removeComp)
                {
                    a->RemoveComponent<T>(index);
                }
            }
        };
    };
}