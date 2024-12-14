#include "Test.h"

void TestScript::OnStart()
{
}

void TestScript::OnUpdate()
{
    auto t = GetTransform();

    if (Input::GetKey(Keys::A))
        t->Position.x += 1;
}
