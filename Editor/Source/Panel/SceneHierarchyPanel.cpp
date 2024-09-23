#include "SceneHierarchyPanel.h"

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

    void SceneHierarchyPanel::RenderActorComponents(Actor *a)
    {
        if (!a)
            return;

        {
            static char buffer[256];
            CeMemory::Zero(&buffer, 256);
            CeMemory::Copy(&buffer, a->GetName().c_str(), a->GetName().size() <= 256 ? a->GetName().size() : 256);
            if (ImGui::InputText("Name", buffer, 256))
                a->SetName(buffer);
        }

        auto t = a->GetTransform();
        float data[3] = {t->Position.x, t->Position.y, t->Position.z};
        if (ImGui::DragFloat3("Position", data))
            t->Position.Set(data[0], data[1], data[2]);

        float data2[3] = {t->Rotation.x * CE_RAD_TO_DEG, t->Rotation.y * CE_RAD_TO_DEG, t->Rotation.z * CE_RAD_TO_DEG};
        if (ImGui::DragFloat3("Rotation", data2, 0.1))
            t->Rotation.Set(data2[0] * CE_DEG_TO_RAD, data2[1] * CE_DEG_TO_RAD, data2[2] * CE_DEG_TO_RAD);
    }
}