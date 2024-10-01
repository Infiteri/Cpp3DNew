#include "EditorCamera.h"

namespace Core
{
    EditorCamera::EditorCamera()
    {
    }

    EditorCamera::~EditorCamera()
    {
    }

    void EditorCamera::UpdateMovement()
    {
    }

    void EditorCamera::UpdateMouse()
    {
        float speedActual = 0.0f;

        if (Input::GetKey(Keys::LeftShift))
            speedActual = speed * 2;
        else
            speedActual = speed;

        if (Input::GetButton(Buttons::Right))
        {
            Input::SetMouseMode(MouseMode::Locked);

            float dt = Engine::GetDeltaTime();

            if (Input::GetKey(Keys::W))
            {
                Matrix4 cameraMatrixRotation = Matrix4::RotationZYX(camera->GetRotation());
                Vector3 way = Matrix4::Forward(&cameraMatrixRotation);
                camera->GetPosition().x += way.x * speedActual;
                camera->GetPosition().y += way.y * speedActual;
                camera->GetPosition().z += way.z * speedActual;
                camera->UpdateView();
            }

            if (Input::GetKey(Keys::S))
            {
                Matrix4 cameraMatrixRotation = Matrix4::RotationZYX(camera->GetRotation());
                Vector3 way = Matrix4::Forward(&cameraMatrixRotation);
                camera->GetPosition().x -= way.x * speedActual;
                camera->GetPosition().y -= way.y * speedActual;
                camera->GetPosition().z -= way.z * speedActual;
                camera->UpdateView();
            }

            if (Input::GetKey(Keys::A))
            {
                Matrix4 cameraMatrixRotation = Matrix4::RotationZYX(camera->GetRotation());
                Vector3 way = Matrix4::Right(&cameraMatrixRotation);
                camera->GetPosition().x -= way.x * speedActual;
                camera->GetPosition().y -= way.y * speedActual;
                camera->GetPosition().z -= way.z * speedActual;
                camera->UpdateView();
            }

            if (Input::GetKey(Keys::D))
            {
                Matrix4 cameraMatrixRotation = Matrix4::RotationZYX(camera->GetRotation());
                Vector3 way = Matrix4::Right(&cameraMatrixRotation);
                camera->GetPosition().x += way.x * speedActual;
                camera->GetPosition().y += way.y * speedActual;
                camera->GetPosition().z += way.z * speedActual;
                camera->UpdateView();
            }

            // Mouse movement
            {
                float deltaX = Input::GetMouseDeltaX();
                float deltaY = Input::GetMouseDeltaY();
                if (deltaX != 0 || deltaY != 0)
                {
                    float sensitivityX = deltaX * sensitivity;
                    float sensitivityY = -deltaY * sensitivity;
                    camera->GetRotation().y += sensitivityX;
                    camera->GetRotation().x += sensitivityY;
                    camera->GetRotation().x = Math::Clamp(Math::DegToRad(-89), Math::DegToRad(89), camera->GetRotation().x);
                }
            }
        }
        else
            Input::SetMouseMode(MouseMode::Visible);
    }
}