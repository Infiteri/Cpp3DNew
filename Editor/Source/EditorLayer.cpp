#include "EditorLayer.h"

#include <imgui.h>

namespace Core
{
    static Core::Mesh *mesh;
    Core::Color setColor;
    static float speed = 1.0f;
    static float rotationSpeed = 1.0f;

    void EditorLayer::OnAttach()
    {
        mesh = new Mesh();
    }

    void EditorLayer::OnImGuiRender()
    {
        float data[4] = {
            mesh->GetMaterial()->GetState().Color.r / 255,
            mesh->GetMaterial()->GetState().Color.g / 255,
            mesh->GetMaterial()->GetState().Color.b / 255,
            mesh->GetMaterial()->GetState().Color.a / 255};

        ImGui::Begin("Test");
        ImGui::ColorEdit4("Color", data);
        setColor.Set(data[0] * 255, data[1] * 255, data[2] * 255, data[3] * 255);

        {
            auto cam = Core::CameraSystem::GetPerspectiveActive();
            if (cam)
            {
                float dataBuff[3] = {cam->GetPosition().x, cam->GetPosition().y, cam->GetPosition().z};
                if (ImGui::DragFloat3("camera position", dataBuff, 0.05f))

                {
                    cam->GetPosition().Set(dataBuff[0], dataBuff[1], dataBuff[2]);
                }

                dataBuff[0] = cam->GetRotation().x;
                dataBuff[1] = cam->GetRotation().y;
                dataBuff[2] = cam->GetRotation().z;

                if (ImGui::DragFloat3("camera rotation", dataBuff, 0.05f))
                {
                    cam->GetRotation().Set(dataBuff[0], dataBuff[1], dataBuff[2]);
                }

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

        ImGui::End();
    }

    void EditorLayer::OnUpdate()
    {
        mesh->Render();

        mesh->GetMaterial()->GetState().Color.Set(setColor);
    }
}