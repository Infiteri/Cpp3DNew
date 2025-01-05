#pragma once

#include "Core.h"
#include "Core/Data/CeDataSet.h"

#include <imgui.h>
#include <string>
#include <typeinfo>

namespace Core
{
    class EditorUtils
    {
    public:
        EditorUtils();
        ~EditorUtils();

        enum TextureType
        {
            TextureColor,
        };

        struct StringEdit
        {
            std::string Content;
            bool Changed;
        };

        static Texture *LoadTexture(const std::string &name);

        static void ImGuiVec2Edit(const char *l, Vector2 *v);
        static void ImGuiVec3Edit(const char *l, Vector3 *v);

        static void ImGuiVector3StyledEdit(const char *label, Vector3 *vec, float defaultValue = 0.0f);
        static void TransformGUIRender(Transform *transform);
        static StringEdit ImGuiStringEdit(const char *label, const std::string &str);
        static std::string ImGuiStringEditReturnString(const char *label, const std::string &str);
        static void ImGuiColorEdit(const char *label, Color *color);
        static void DrawTextureUI(const char *label, TextureType texType, Material *mat);
        static void DrawTextureUIChangeFilter(Texture *texture);

        static bool DrawDataSetUI(CeDataSet *set);
        static void SetSetIDTo0();

        template <typename T, typename UIFun>
        static void DrawComponentBaseUI(const std::string &name, T *component, int index, Actor *a, UIFun fun)
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