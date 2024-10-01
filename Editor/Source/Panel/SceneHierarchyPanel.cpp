#include "SceneHierarchyPanel.h"
#include "EditorUtils.h"

#define CE_UTIL_ADD_RENDER(name, type, cb)                                           \
    int index##type = -1;                                                            \
    for (auto Component##type : a->GetComponents<type>())                            \
    {                                                                                \
        index##type++;                                                               \
        ImGui::NewLine();                                                            \
        EditorUtils::DrawComponentBaseUI(name, Component##type, index##type, a, cb); \
    }

namespace Core
{
    SceneHierarchyPanel::SceneHierarchyPanel()
    {
        selectionContext = nullptr;
    }

    SceneHierarchyPanel::~SceneHierarchyPanel()
    {
    }

    void SceneHierarchyPanel::OnImGuiRender(PanelInformation *information)
    {
        ImGui::Begin("Scene Hierarchy");

        auto scene = World::GetActiveScene();
        if (!scene)
        {
            ImGui::End();
            return;
        }

        for (auto actor : scene->GetActors())
            RenderActor(actor);

        ImGui::End();

        ImGui::Begin("Proprieties");

        if (Input::GetKey(Keys::Escape))
            selectionContext = nullptr;

        if (selectionContext)
            RenderActorComponents(selectionContext);

        ImGui::End();
    }

    void SceneHierarchyPanel::RenderActor(Actor *a)
    {
        if (!a || a == nullptr)
            return;

        // Flags setup
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap;
        if (selectionContext != nullptr)
            if (selectionContext->GetUUID() == a->GetUUID())
                flags |= ImGuiTreeNodeFlags_Selected;

        bool pop = ImGui::TreeNodeEx((void *)(CeU64)(CeU32)a->GetUUID(), flags, a->GetName().c_str());

        if (ImGui::IsItemClicked())
            selectionContext = a;

        if (pop)
            ImGui::TreePop();
    }

    void RenderMeshUI(MeshComponent *mesh, Actor *a);

    void SceneHierarchyPanel::RenderActorComponents(Actor *a)
    {
        if (!a)
            return;

        {
            std::string name = EditorUtils::ImGuiStringEdit("Name", a->GetName());
            a->SetName(name);
        }

        ImGui::Dummy({0, 10});

        if (ImGui::TreeNode("Transform"))
        {
            EditorUtils::TransformGUIRender(a->GetTransform());
            ImGui::TreePop();
        }

        CE_UTIL_ADD_RENDER("Mesh Component", MeshComponent, RenderMeshUI);
    }

    void RenderMeshUI(MeshComponent *mesh, Actor *a)
    {
        if (ImGui::TreeNode("Material"))
        {
            auto mat = mesh->mesh->GetMaterial();

            // UI Editing;
            {
                switch (mat->GetType())
                {
                case Material::Config:
                    EditorUtils::ImGuiColorEdit("Color", &mat->GetState().Color);

                    if (ImGui::TreeNode("Color Texture"))
                    {
                        EditorUtils::DrawTextureUI("Color Texture", EditorUtils::TextureColor, mat);
                        ImGui::TreePop();
                    }
                    break;

                case Material::Default:
                default:
                    ImGui::Text("Material is default.");
                    break;
                }
            }

            // UI Button
            {

                switch (mat->GetType())
                {
                case Material::Config:
                    if (ImGui::Button("Make Default"))
                    {
                        mesh->mesh->MakeMaterialDefault();
                    }
                    break;

                case Material::Default:
                default:
                    if (ImGui::Button("Make Unique"))
                    {
                        mesh->mesh->MakeMaterialUnique();
                    }
                    break;
                }
            }

            ImGui::TreePop();
        }
    }

}