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

        actorIndex = 0;
        for (auto actor : scene->GetActors())
        {
            RenderActor(actor, nullptr, false);
            actorIndex++;
        }

        ImGui::End();

        ImGui::Begin("Proprieties");

        if (Input::GetKey(Keys::Escape))
            selectionContext = nullptr;

        if (selectionContext)
            RenderActorComponents(selectionContext);

        ImGui::End();
    }

    void SceneHierarchyPanel::RenderActor(Actor *a, Actor *parent, bool parentNodeOpen)
    {
        if (!a || a == nullptr)
            return;

        // Flags setup
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap;
        if (selectionContext != nullptr)
            if (selectionContext->GetUUID() == a->GetUUID())
                flags |= ImGuiTreeNodeFlags_Selected;

        bool pop = false;

        if (!parent)
        {
            pop = ImGui::TreeNodeEx((void *)(CeU64)(CeU32)a->GetUUID(), flags, a->GetName().c_str());
            if (ImGui::IsItemClicked())
                selectionContext = a;
        }
        else
        {
            if (parentNodeOpen)
            {
                pop = ImGui::TreeNodeEx((void *)(CeU64)(CeU32)a->GetUUID(), flags, a->GetName().c_str());
                if (ImGui::IsItemClicked())
                    selectionContext = a;
            }
        }

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            auto uuid = a->GetUUID();
            ImGui::SetDragDropPayload("CE_SCENE_HIERARCHY_ACTOR", &uuid, sizeof(UUID));
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_SCENE_HIERARCHY_ACTOR"))
            {
                UUID *uid = (UUID *)payload->Data;
                UUID rawUUID = UUID(uid->Get());

                CE_CORE_TRACE("%uul to %ull", a->GetUUID().Get(), uid->Get());

                // NOTE: Actor is target, data is name of the actor to move
                auto moveActor = World::GetActiveScene()->GetActorInHierarchy(rawUUID);
                if (moveActor)
                {
                    if (!moveActor->FindChildByUUID(a->GetUUID()))
                    {
                        if (moveActor->GetParent())
                            moveActor->GetParent()->RemoveActorByUUID(rawUUID);
                        else
                            World::GetActiveScene()->RemoveActorByUUID(rawUUID);
                        a->AddChild(moveActor);
                    }
                }
                else
                    CE_ERROR("Un-found actor.");
            }

            ImGui::EndDragDropTarget();
        }

        if (!parent)
        {
            ImGui::Dummy({ImGui::GetWindowWidth(), 5});
        }
        else
        {
            if (parentNodeOpen)
                ImGui::Dummy({ImGui::GetWindowWidth(), 5});
        }

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            auto uuid = a->GetUUID();
            ImGui::SetDragDropPayload("CE_SCENE_HIERARCHY_ACTOR", &uuid, sizeof(UUID));
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_SCENE_HIERARCHY_ACTOR"))
            {
                UUID *uid = (UUID *)payload->Data;
                UUID rawUUID = UUID(uid->Get());

                CE_CORE_TRACE("%uul to %ull", a->GetUUID().Get(), uid->Get());

                // NOTE: Actor is target, data is name of the actor to move
                auto moveActor = World::GetActiveScene()->GetActorInHierarchy(rawUUID);
                if (moveActor)
                {
                    // Both actor are in the scene and not parented
                    if (!moveActor->GetParent())
                        World::GetActiveScene()->MoveChildInHierarchy(moveActor->GetUUID(), actorIndex);
                    else
                    {
                        moveActor->GetParent()->RemoveActorByUUID(moveActor->GetUUID()); // remove from parent and add to scene
                        World::GetActiveScene()->AddActor(moveActor);
                        World::GetActiveScene()->MoveChildInHierarchy(moveActor->GetUUID(), actorIndex);
                    }
                }
            }

            ImGui::EndDragDropTarget();
        }

        for (Actor *child : a->GetChildren())
            RenderActor(child, a, pop);

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