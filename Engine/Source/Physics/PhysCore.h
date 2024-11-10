#pragma once

#include "Core/Engine.h"
#include "PhysicsEngine.h"

#define CE_PHYSICS_FPS (Core::PhysicsEngine::GetNumericValueSet().MathFPS)
#define CE_DELTA_TIME (Core::Engine::GetDeltaTime())
#define CE_PHYSICS_DELTA_TIME (Core::Engine::GetDeltaTime() / (CE_PHYSICS_FPS / 60))
