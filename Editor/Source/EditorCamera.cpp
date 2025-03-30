#include "EditorCamera.h"

namespace Core
{
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
                Matrix4 rotation = Matrix4::RotationZYX(camera->GetRotation());
                Vector3 way = Matrix4::Forward(&rotation);
                camera->GetPosition() += way * speedActual;
                camera->UpdateView();
            }

            if (Input::GetKey(Keys::S))
            {
                Matrix4 rotation = Matrix4::RotationZYX(camera->GetRotation());
                Vector3 way = Matrix4::Forward(&rotation);
                camera->GetPosition() -= way * speedActual;
                camera->UpdateView();
            }

            if (Input::GetKey(Keys::A))
            {
                Matrix4 rotation = Matrix4::RotationZYX(camera->GetRotation());
                Vector3 way = Matrix4::Right(&rotation);
                camera->GetPosition() -= way * speedActual;
                camera->UpdateView();
            }

            if (Input::GetKey(Keys::D))
            {
                Matrix4 rotation = Matrix4::RotationZYX(camera->GetRotation());
                Vector3 way = Matrix4::Right(&rotation);
                camera->GetPosition() += way * speedActual;
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