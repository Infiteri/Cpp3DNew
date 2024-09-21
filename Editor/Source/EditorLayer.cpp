#include "EditorLayer.h"

#include <imgui.h>

namespace Core
{
    static float speed = 1.0f;
    static float rotationSpeed = 1.0f;

    static Scene *scene;

    void EditorLayer::OnAttach()
    {
        scene = World::CreateScene("Scene");
        World::ActivateScene("Scene");

        auto testActor = scene->SpawnActor();
        auto mesh = testActor->AddComponent<MeshComponent>();
    }

    void EditorLayer::OnImGuiRender()
    {

        // TODO: Move to editor layer camera class
        {
            auto cam = Core::CameraSystem::GetPerspectiveActive();
            if (cam)
            {
                cam->UpdateView();

                if (Input::GetKey(Keys::W))
                    cam->GetPosition().z -= speed;

                if (Input::GetKey(Keys::S))
                    cam->GetPosition().z += speed;

                if (Input::GetKey(Keys::A))
                    cam->GetPosition().x -= speed;

                if (Input::GetKey(Keys::D))
                    cam->GetPosition().x += speed;

                if (Input::GetKey(Keys::Q))
                    cam->GetPosition().x += speed;

                if (Input::GetKey(Keys::E))
                    cam->GetPosition().x -= speed;

                if (Input::GetKey(Keys::I))
                    cam->GetRotation().x += rotationSpeed * CE_DEG_TO_RAD;

                if (Input::GetKey(Keys::K))
                    cam->GetRotation().x -= rotationSpeed * CE_DEG_TO_RAD;

                if (Input::GetKey(Keys::J))
                    cam->GetRotation().y -= rotationSpeed * CE_DEG_TO_RAD;

                if (Input::GetKey(Keys::L))
                    cam->GetRotation().y += rotationSpeed * CE_DEG_TO_RAD;
            }
        }
    }

    void EditorLayer::OnUpdate()
    {
        World::UpdateActiveScene(); // TODO: Add this to the engine somehow please thank you :)
    }
}