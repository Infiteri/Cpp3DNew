#include "Mesh.h"
#include "Renderer/Shader/Shader.h"

namespace Core
{

    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    Mesh::Mesh()
    {

        vertex = new VertexArray();

        vertex->GenVertexBuffer(vertices, sizeof(vertices) * sizeof(float));
        vertex->GetVertexBuffer()->AddLayout(0, 0, 3);
        vertex->GenIndexBuffer(indices, sizeof(indices) * sizeof(CeU32));
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::Render()
    {
        vertex->Bind();
        vertex->GetVertexBuffer()->Bind();
        vertex->GetIndexBuffer()->Draw();
    }
}