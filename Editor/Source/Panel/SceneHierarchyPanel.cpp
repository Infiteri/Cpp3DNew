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

#define CE_ADD_COMPONENT(type)  \
    if (ImGui::MenuItem(#type)) \
    selectionContext->AddComponent<type##Component>()

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

        if (ImGui::BeginPopupContextWindow(0, 1))
        {
            if (ImGui::MenuItem("Create Actor"))
                scene->SpawnActor();

            ImGui::EndPopup();
        }

        actorIndex = 0;
        for (auto actor : scene->GetActors())
        {
            RenderActor(actor, nullptr, false);
            actorIndex++;
        }

        ImGui::End();

        EditorUtils::SetSetIDTo0();

        ImGui::Begin("Proprieties");

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
        { // todo
        }

        if (Input::GetKey(Keys::Escape))
            selectionContext = nullptr;

        if (selectionContext)
            RenderActorComponents(selectionContext);

        ImGui::End();
    }

    void SceneHierarchyPanel::RenderActor(Actor *a, Actor *parent, bool parentNodeOpen)
    {
        CE_VERIFY(a || a != nullptr);

        // Flags setup
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap;
        if (selectionContext != nullptr)
            if (selectionContext->SameUUID(a))
                flags |= ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

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

                // NOTE: Actor is target, data is name of the actor to move
                auto moveActor = World::GetActiveScene()->GetActorInHierarchy(rawUUID);
                if (moveActor)
                {
                    if (!moveActor->FindChild(a->GetUUID()))
                    {
                        if (moveActor->GetParent())
                            moveActor->GetParent()->RemoveActor(rawUUID);
                        else
                            World::GetActiveScene()->RemoveActor(rawUUID);
                        a->AddChild(moveActor);
                    }
                }
                else
                    CE_ERROR("Un-found actor.");
            }

            ImGui::EndDragDropTarget();
        }

        if (!parent)
            ImGui::Dummy({ImGui::GetWindowWidth(), 5});
        else if (parentNodeOpen)
            ImGui::Dummy({ImGui::GetWindowWidth(), 5});

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

                // NOTE: Actor is target, data is name of the actor to move
                auto moveActor = World::GetActiveScene()->GetActorInHierarchy(rawUUID);
                if (moveActor)
                {
                    // Both actor are in the scene and not parented
                    if (!moveActor->GetParent())
                        World::GetActiveScene()->MoveChildInHierarchy(moveActor->GetUUID(), actorIndex);
                    else
                    {
                        moveActor->GetParent()->RemoveActor(moveActor->GetUUID()); // remove from parent and add to scene
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

    void RenderMeshUI(MeshComponent *c, Actor *a);
    void RenderDataSetUI(DataSetComponent *c, Actor *a);
    void RenderScriptUI(ScriptComponent *c, Actor *a);
    void RenderCameraUI(CameraComponent *c, Actor *a);
    void RenderPointLightUI(PointLightComponent *c, Actor *a);
    void RenderRigidBodyUI(RigidBodyComponent *c, Actor *a);
    void RenderTagUI(TagComponent *c, Actor *a);
    void RenderStaticBodyUI(StaticBodyComponent *c, Actor *a);
    void RenderBoxColliderUI(BoxColliderComponent *c, Actor *a);

    void SceneHierarchyPanel::RenderActorComponents(Actor *a)
    {
        CE_VERIFY(a);

        {
            auto name = EditorUtils::ImGuiStringEdit("Name", a->GetName());
            if (name.Changed)
                a->SetName(name.Content);
        }

        ImGui::Dummy({0, 10});

        if (ImGui::TreeNode("Transform"))
        {
            EditorUtils::TransformGUIRender(a->GetTransform());
            ImGui::TreePop();
        }

        CE_UTIL_ADD_RENDER("Tag Component", TagComponent, RenderTagUI);
        CE_UTIL_ADD_RENDER("Mesh Component", MeshComponent, RenderMeshUI);
        CE_UTIL_ADD_RENDER("Data Set Component", DataSetComponent, RenderDataSetUI);
        CE_UTIL_ADD_RENDER("Script Component", ScriptComponent, RenderScriptUI);
        CE_UTIL_ADD_RENDER("Camera Component", CameraComponent, RenderCameraUI);
        CE_UTIL_ADD_RENDER("Point Light Component", PointLightComponent, RenderPointLightUI);
        CE_UTIL_ADD_RENDER("Rigid Body Component", RigidBodyComponent, RenderRigidBodyUI);
        CE_UTIL_ADD_RENDER("Static Body Component", StaticBodyComponent, RenderStaticBodyUI);
        CE_UTIL_ADD_RENDER("Box Collider Component", BoxColliderComponent, RenderBoxColliderUI);

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("ComponentPopup");

        if (ImGui::BeginPopupContextItem("ComponentPopup"))
        {
            CE_ADD_COMPONENT(Tag);
            CE_ADD_COMPONENT(Mesh);
            CE_ADD_COMPONENT(DataSet);
            CE_ADD_COMPONENT(Script);
            CE_ADD_COMPONENT(Camera);
            CE_ADD_COMPONENT(PointLight);
            CE_ADD_COMPONENT(RigidBody);
            CE_ADD_COMPONENT(StaticBody);
            CE_ADD_COMPONENT(BoxCollider);
            ImGui::EndPopup();
        }
    }

    void RenderMeshUI(MeshComponent *mesh, Actor *a)
    {
        if (ImGui::TreeNode("Material"))
        {
            auto mat = mesh->mesh->GetMaterial();

            // Type editing
            {
                const int maxSelections = 3;
                const char *selections[maxSelections] = {"Default", "Config", "File"};
                const char *current = selections[(int)mat->GetType()];

                if (ImGui::BeginCombo("Type", current))
                {
                    for (int i = 0; i < maxSelections; i++)
                    {
                        bool isSelected = (current == selections[i]);

                        if (ImGui::Selectable(selections[i], isSelected))
                        {
                            current = selections[i];

                            switch ((Material::MaterialTypes)i)
                            {
                            case Material::Default:
                                mesh->mesh->MakeMaterialDefault();
                                break;

                            case Material::Config:
                                mesh->mesh->MakeMaterialUnique();
                                break;

                            case Material::File:
                                mesh->mesh->MakeMaterialFromFile(); // Note, until user decides to load a config file
                                break;
                            }
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }
            }

            // UI Editing;
            {
                switch (mat->GetType())
                {
                case Material::Config:

                    if (ImGui::TreeNode("Color"))
                    {
                        EditorUtils::ImGuiColorEdit("Color", &mat->GetState().Color);

                        if (ImGui::TreeNode("Texture"))
                        {
                            EditorUtils::DrawTextureUI("Color Texture", EditorUtils::TextureColor, mat);
                            ImGui::TreePop();
                        }

                        ImGui::TreePop();
                    }
                    break;

                case Material::File:
                    if (ImGui::Button("Load"))
                    {
                        std::string path = Platform::OpenFileDialog("Material.ce_material \0*.ce_material\0");
                        if (!path.empty())
                            mesh->mesh->MakeMaterialFromFile(path);
                    }

                    if (ImGui::Button("ReLoad"))
                    {
                        if (!mat->GetFilePath().empty())
                            mesh->mesh->MakeMaterialFromFile(mat->GetFilePath());
                    }

                    break;

                case Material::Default:
                default:
                    ImGui::Text("Material is default.");
                    break;
                }
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Geometry"))
        {
            auto g = mesh->mesh->GetGeometry();

            switch (g->GetType())
            {
            case Geometry::Box:
            {
                auto b = (BoxGeometry *)g;

                float data[3] = {b->Width, b->Height, b->Depth};
                if (ImGui::DragFloat3("Size", data, 0.05f, 0.0f))
                    mesh->mesh->SetGeometry(new BoxGeometry(data[0], data[1], data[2]));

                // NOTE: If the geometry changes, the address changes as well, faults if this isn't done
                g = mesh->mesh->GetGeometry();
            }
            break;

            case Geometry::Sphere:
            {

                auto b = (SphereGeometry *)g;

                // edit the values, important that its done this

                if (ImGui::DragFloat("Radius", &b->Radius, 0.05f, 0.0f))
                    mesh->mesh->SetGeometry(new SphereGeometry(b->Radius, b->Subdivision));

                if (ImGui::DragInt("Subdivision", &b->Subdivision, 0.05f, 0))
                    mesh->mesh->SetGeometry(new SphereGeometry(b->Radius, b->Subdivision));

                // NOTE: If the geometry changes, the address changes as well, faults if this isn't done
                g = mesh->mesh->GetGeometry();
            }
            break;
            }

            // -- Selection for the geometry types --
            {
                const int Count = 3;
                const char *Type[Count] = {"None", "Box", "Sphere"};
                const char *Current = Type[g->GetType()];

                if (ImGui::BeginCombo("Type", Current))
                {
                    for (int type = 0; type < Count; type++)
                    {
                        bool isSelected = (Current == Type[type]);
                        if (ImGui::Selectable(Type[type], isSelected))
                        {
                            Current = Type[type];

                            switch ((Geometry::Types)type)
                            {
                            case Geometry::Box:
                                mesh->mesh->SetGeometry(new BoxGeometry());
                                break;

                            case Geometry::Sphere:
                                mesh->mesh->SetGeometry(new SphereGeometry());
                                break;

                            default:
                                mesh->mesh->SetGeometry(new Geometry());
                                break;
                            }
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }
            }

            ImGui::TreePop();
        }
    }

    void RenderDataSetUI(DataSetComponent *c, Actor *a)
    {
        EditorUtils::DrawDataSetUI(&c->Set);

        if (ImGui::Button("Add"))
        {
            c->Set.Add(new CeData(new Vector2(), CeData::DataVec2, "Vector 2"));
        }
    }

    void RenderScriptUI(ScriptComponent *c, Actor *a)
    {
        auto scriptChange = EditorUtils::ImGuiStringEdit("Class Name", c->ClassName);
        if (scriptChange.Changed)
            c->ClassName = scriptChange.Content;
    }

    void RenderCameraUI(CameraComponent *c, Actor *a)
    {
        ImGui::DragFloat("FOV", &c->FOV, 0.05f, 0.0f, 360.0f);
        ImGui::DragFloat("Near", &c->Near, 0.05f, 0.01f);
        ImGui::DragFloat("Far", &c->Far, 0.05f, 0.01f);

        bool primary = c->IsPrimary;
        if (ImGui::Checkbox("Is Primary", &primary))
        {
            World::GetActiveScene()->SetPrimaryCameraToNone(); // Automatically tick off every single camera to be non primary
            c->IsPrimary = primary;
        }
    }

    void RenderPointLightUI(PointLightComponent *c, Actor *a)
    {
        EditorUtils::ImGuiVec3Edit("Specular", &c->Light->Specular);
        EditorUtils::ImGuiColorEdit("Color", &c->Light->Color);

        ImGui::DragFloat("Constant", &c->Light->Constant, 0.05f, 0.0f);
        ImGui::DragFloat("Quadratic", &c->Light->Quadratic, 0.05f, 0.0f);
        ImGui::DragFloat("Linear", &c->Light->Linear, 0.05f, 0.0f);
        ImGui::DragFloat("Radius", &c->Light->Radius, 0.05f, 0.0f);
        ImGui::DragFloat("Intensity", &c->Light->Intensity, 0.05f, 0.0f);
    }

    void RenderTagUI(TagComponent *c, Actor *a)
    {
        auto tagChange = EditorUtils::ImGuiStringEdit("Tag", c->Tag);
        if (tagChange.Changed)
            c->Tag = tagChange.Content;
    }

    void RenderRigidBodyUI(RigidBodyComponent *c, Actor *a)
    {
        ImGui::DragFloat("Linear Damp", &c->Config.LinearDamp, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat("Angular Damp", &c->Config.AngularDamp, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat("Mass", &c->Config.Mass, 0.05f, 0.0f);
    }

    void RenderStaticBodyUI(StaticBodyComponent *c, Actor *a)
    {
        ImGui::DragFloat("Mass", &c->Config.Mass, 0.05f, 0.0f);
    }

    void RenderBoxColliderUI(BoxColliderComponent *c, Actor *a)
    {
        float data[3] = {c->Width, c->Height, c->Depth};

        if (ImGui::DragFloat3("Size", data, 0.05f, 0.0f))
        {
            c->Width = data[0];
            c->Height = data[1];
            c->Depth = data[2];
        }
    }
}