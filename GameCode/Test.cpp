#include "Test.h"

void TestScript::OnStart()
{
}

void TestScript::OnUpdate()
{
    auto rc = GetComponent<RigidBodyComponent>();
    if (!rc)
        return;

    if (Input::GetKey(Keys::A))
        rc->BodyInstance->AddForce({-1, 0, 0});

    if (Input::GetKey(Keys::D))
        rc->BodyInstance->AddForce({1, 0, 0});

    if (Input::GetKey(Keys::Q))
        rc->BodyInstance->AddTorque({0, 0, 10});

    if (Input::GetKey(Keys::R))
        rc->BodyInstance->AddTorque({-10, 0, 0});
}
