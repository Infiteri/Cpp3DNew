#pragma once

#include "Core/Engine.h"
#include "PhysicsEngine.h"

#define CE_PHYSICS_FPS (Core::PhysicsEngine::GetNumericValueSet().MathFPS)
#define CE_DELTA_TIME (Core::Engine::GetDeltaTime())
#define CE_PHYSICS_DELTA_TIME (Core::Engine::GetDeltaTime() * (60 / CE_PHYSICS_FPS))
