#include "VertexArray.h"

#include <glad/glad.h>

namespace Core
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &id);
        glBindVertexArray(id);

        vertexBuffer = indexBuffer = nullptr;
    }

    VertexArray::~VertexArray()
    {
        Destroy();
    }

    void VertexArray::Destroy()
    {
        glDeleteVertexArrays(1, &id);

        if (vertexBuffer)
            delete vertexBuffer;

        if (indexBuffer)
            delete indexBuffer;

        vertexBuffer = indexBuffer = nullptr;
    }

    void VertexArray::Bind()
    {
        glBindVertexArray(id);
    }

    void VertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    CeU32 VertexArray::GetID()
    {
        return id;
    }

    Buffer *VertexArray::GenVertexBuffer(void *data, CeU64 dataSize)
    {
        if (vertexBuffer)
            delete vertexBuffer;

        vertexBuffer = new Buffer(BufferType::Vertex);
        vertexBuffer->UploadData(data, dataSize);

        return vertexBuffer;
    }

    Buffer *VertexArray::GenIndexBuffer(void *data, CeU64 dataSize)
    {
        if (indexBuffer)
            delete indexBuffer;

        indexBuffer = new Buffer(BufferType::Index);
        indexBuffer->UploadData(data, dataSize);

        return indexBuffer;
    }

    Buffer *VertexArray::GetVertexBuffer()
    {
        return vertexBuffer;
    }

    Buffer *VertexArray::GetIndexBuffer()
    {
        return indexBuffer;
    }
}