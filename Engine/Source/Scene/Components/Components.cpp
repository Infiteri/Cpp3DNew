#include "Components.h"
#include "Scene/Actor.h"

namespace Core
{
    MeshComponent::MeshComponent()
    {
        mesh = new Mesh();
    }

    void MeshComponent::Destroy()
    {
        delete mesh;
    }

    void MeshComponent::Render()
    {
        mesh->Render();
    }

    void MeshComponent::From(MeshComponent *c)
    {
    }

    DataSetComponent::DataSetComponent()
    {
    }

    DataSetComponent::~DataSetComponent()
    {
    }

    void DataSetComponent::From(DataSetComponent *c)
    {
        for (auto d : c->Set.GetSet())
        {
            CeData *c = new CeData();
            c->From(d);
            Set.Add(c);
        }
    }
}