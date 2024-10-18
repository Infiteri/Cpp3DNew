#pragma once

#include "Base.h"
#include "Renderer/Object/Mesh.h"
#include "Core/Data/CeDataSet.h"

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

}