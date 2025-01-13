#pragma once

#include "Base.h"
#include "Renderer/Object/Mesh.h"
#include "Core/Data/CeDataSet.h"
#include "Renderer/Camera/PerspectiveCamera.h"
#include "Renderer/Light/PointLight.h"

#include "Physics/Body/RigidBody.h"
#include "Physics/Body/StaticBody.h"
#include "Physics/Body/KinematicBody.h"

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
        friend class Scene;

        // Note: For editor
        friend class EditorLayer;

    public:
        /// @brief Wether or not to ignore 'SetViewMatrix' on the Camera Instance.
        /// @note This is not serialized / deserialized as its a runtime flag.
        /// @note Set to true if you plan an updating the camera matrix on your own.
        /// @todo Implement a better system.
        bool IgnoreMatrixUpload = false;

        PerspectiveCamera *Camera;
        float FOV = 90.0f;
        float Far = 1000.0f;
        float Near = 0.01f;
        bool IsPrimary = true;

        CameraComponent();
        ~CameraComponent();

        void Render();

        void UpdateCameraState();

        /// @brief Will correctly clamp a rotation angle of the camera.
        /// @brief Due the nature of angles, and them going 0 - 360 range, clamping can get tricky. This function does however make it easy.
        /// @param angle The angle to clamp, in degrees.
        /// @param degree The degree to clamp the angle at (a.k.a. its limit).
        /// @return The correct and clamped angle.
        float ClampRotation(float angle, float degree = 89);

        enum Angles
        {
            X = 1 << 0,
            Y = 1 << 1,
            Z = 1 << 2
        };

        /// @brief Will correctly clamp the specified rotation angle of the target.
        /// @param angles The angles to clamp.
        /// @param degree The degree to clamp at (a.k.a. limit).
        /// @param isOnActor Wether or not to clamp actor rotation or camera rotation, goes with 'IgnoreMatrixUpload' in some way. todo: test with camera
        void ClampRotation(Angles angles, float degree = 89, bool isOnActor = true);

        void From(CameraComponent *c);
    };

    class CE_API PointLightComponent : public Component
    {
    public:
        PointLight *Light;

        PointLightComponent();
        ~PointLightComponent();

        void Render();

        void From(PointLightComponent *c);
    };

    class CE_API TagComponent : public Component
    {
    public:
        std::string Tag;

        TagComponent();
        TagComponent(const std::string &tag) : Tag(tag) {};
        ~TagComponent();

        void From(TagComponent *c);
    };

    class CE_API RigidBodyComponent : public Component
    {
    public:
        RigidBodyConfiguration Config;
        RigidBody *BodyInstance;

        RigidBodyComponent();
        ~RigidBodyComponent();

        void From(RigidBodyComponent *c);
    };

    class CE_API StaticBodyComponent : public Component
    {
    public:
        StaticBodyConfiguration Config;
        StaticBody *BodyInstance;

        StaticBodyComponent();
        ~StaticBodyComponent();

        void From(StaticBodyComponent *c);
    };

    class CE_API KinematicBodyComponent : public Component
    {
    public:
        KinematicBodyConfiguration Config;
        KinematicBody *BodyInstance;

        KinematicBodyComponent();
        ~KinematicBodyComponent();

        void From(KinematicBodyComponent *c);
    };

    class CE_API ColliderComponent : public Component
    {
    public:
        ColliderComponent() {};
        ~ColliderComponent() {};
    };

    class CE_API BoxColliderComponent : public ColliderComponent
    {
    public:
        float Width = 1.0f;
        float Height = 1.0f;
        float Depth = 1.0f;

        BoxColliderComponent() {};
        ~BoxColliderComponent() {};

        void From(BoxColliderComponent *c);
    };
}