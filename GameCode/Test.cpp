#include "Test.h"

void TestScript::OnStart()
{
    CE_TRACE("Test actor");
}

void TestScript::OnUpdate()
{
    Owner->GetTransform()->Position.x++;
}
