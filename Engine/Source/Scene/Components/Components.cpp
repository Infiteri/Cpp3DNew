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

}