#include "Test.h"

void TestScript::OnStart()
{
}

void TestScript::OnUpdate()
{
    auto t = GetTransform();
    auto ec = GetComponent<RigidBodyComponent>();

    if (!ec)
    {
        return;
    }

    if (Input::GetKey(Keys::A))
    {
        ec->BodyInstance->AddForce({100, 0, 0});
    }
}
