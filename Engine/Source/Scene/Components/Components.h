#pragma once

#include "Base.h"
#include "Renderer/Object/Mesh.h"
#include "Core/Data/CeDataSet.h"
#include "Renderer/Camera/PerspectiveCamera.h"

namespace Core
{
    class CE_API Component
    {
    public:
        class Actor *Owner = nullptr;

        Component() {};
        virtual ~Component() { Destroy(); };

        virtual void Destroy() {};

        virtual void Start() {};
        virtual void Render() {};
        virtual void Update() {};
        virtual void Stop() {};

        virtual void From(Component *c) {};
    };

    class CE_API MeshComponent : public Component
    {
    public:
        Mesh *mesh;
        MeshComponent();
        ~MeshComponent() { Destroy(); };

        void Destroy();
        void Render();

        void From(MeshComponent *c);
    };

    class CE_API DataSetComponent : public Component
    {
    public:
        CeDataSet Set;

        DataSetComponent();
        ~DataSetComponent();

        void From(DataSetComponent *c);
    };

    class CE_API ScriptComponent : public Component
    {
    public:
        std::string ClassName;

        ScriptComponent();
        ~ScriptComponent();

        void From(ScriptComponent *c);
    };

    class CE_API CameraComponent : public Component
    {
    private:
        PerspectiveCamera *Camera;

        friend class Scene;

        // Note: For editor
        friend class EditorLayer;

    public:
        float FOV = 90.0f;
        float Far = 1000.0f;
        float Near = 0.01f;
        bool IsPrimary = true;

        CameraComponent();
        ~CameraComponent();

        void Render();

        void UpdateCameraState();

        void From(CameraComponent *c);
    };
}