#pragma once

#include "Base.h"

#include "Core/Application.h"
#include "Core/CeAssert.h"
#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Core/Window.h"
#include "Core/Input.h"
#include "Core/Layer/Layer.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Memory/CeMemory.h"
#include "Core/Layer/ImGuiLayer.h"

#include "Math/Math.h"
#include "Math/Matrix4.h"
#include "Math/Transform.h"
#include "Math/Vectors.h"

#include "Platform/Platform.h"

#include "Renderer/Color.h"
#include "Renderer/Renderer.h"
#include "Renderer/Material/Material.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Shader/ShaderSystem.h"
#include "Renderer/Object/Mesh.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Camera/Camera.h"
#include "Renderer/Camera/PerspectiveCamera.h"
#include "Renderer/Texture/BaseTexture.h"
#include "Renderer/Texture/Texture.h"
#include "Renderer/Texture/TextureSystem.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/World.h"
#include "Scene/Components/Components.h"
#include "Scene/Serializer/SceneSerializer.h"

#include "Resources/Image.h"
#include "Resources/Loaders/CubeMapLoader.h"

#include "Script/ActorScript.h"

#include "Physics/PhysicsEngine.h"
#include "Physics/Objects/Spring.h"