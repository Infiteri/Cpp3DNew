#pragma once

#include "Base.h"
#include "Renderer/Object/Mesh.h"

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
    };

    class CE_API MeshComponent : public Component
    {
    public:
        Mesh *mesh;
        MeshComponent();
        ~MeshComponent() { Destroy(); };

        void Destroy();
        void Render();
    };

}