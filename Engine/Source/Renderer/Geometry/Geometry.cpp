#include "Geometry.h"

namespace Core
{
    BoxGeometry::BoxGeometry(float width, float height, float depth)
    {
        Width = width;
        Height = height;
        Depth = depth;
        float vertexCount = 4 * 6;
        // 4 verts per segment
        Vertices.resize(vertexCount);
        float indexCount = 6 * 6;
        // 6 indices per segment
        Indices.resize(indexCount);

        float min_x = -width;
        float min_y = -height;
        float min_z = -depth;
        float max_x = width;
        float max_y = height;
        float max_z = depth;
        float min_uvx = 0.0f;
        float min_uvy = 0.0f;
        float max_uvx = 1;
        float max_uvy = 1;

        Vertices[(0 * 4) + 0].Position.Set(min_x, min_y, max_z);
        Vertices[(0 * 4) + 1].Position.Set(max_x, max_y, max_z);
        Vertices[(0 * 4) + 2].Position.Set(min_x, max_y, max_z);
        Vertices[(0 * 4) + 3].Position.Set(max_x, min_y, max_z);
        Vertices[(0 * 4) + 0].UV.Set(min_uvx, min_uvy);
        Vertices[(0 * 4) + 1].UV.Set(max_uvx, max_uvy);
        Vertices[(0 * 4) + 2].UV.Set(min_uvx, max_uvy);
        Vertices[(0 * 4) + 3].UV.Set(max_uvx, min_uvy);
        Vertices[(0 * 4) + 0].Normal.Set(0.0f, 0.0f, 1.0f);
        Vertices[(0 * 4) + 1].Normal.Set(0.0f, 0.0f, 1.0f);
        Vertices[(0 * 4) + 2].Normal.Set(0.0f, 0.0f, 1.0f);
        Vertices[(0 * 4) + 3].Normal.Set(0.0f, 0.0f, 1.0f);
        Vertices[(1 * 4) + 0].Position.Set(max_x, min_y, min_z);
        Vertices[(1 * 4) + 1].Position.Set(min_x, max_y, min_z);
        Vertices[(1 * 4) + 2].Position.Set(max_x, max_y, min_z);
        Vertices[(1 * 4) + 3].Position.Set(min_x, min_y, min_z);
        Vertices[(1 * 4) + 0].UV.Set(min_uvx, min_uvy);
        Vertices[(1 * 4) + 1].UV.Set(max_uvx, max_uvy);
        Vertices[(1 * 4) + 2].UV.Set(min_uvx, max_uvy);
        Vertices[(1 * 4) + 3].UV.Set(max_uvx, min_uvy);
        Vertices[(1 * 4) + 0].Normal.Set(0.0f, 0.0f, -1.0f);
        Vertices[(1 * 4) + 1].Normal.Set(0.0f, 0.0f, -1.0f);
        Vertices[(1 * 4) + 2].Normal.Set(0.0f, 0.0f, -1.0f);
        Vertices[(1 * 4) + 3].Normal.Set(0.0f, 0.0f, -1.0f);
        Vertices[(2 * 4) + 0].Position.Set(min_x, min_y, min_z);
        Vertices[(2 * 4) + 1].Position.Set(min_x, max_y, max_z);
        Vertices[(2 * 4) + 2].Position.Set(min_x, max_y, min_z);
        Vertices[(2 * 4) + 3].Position.Set(min_x, min_y, max_z);
        Vertices[(2 * 4) + 0].UV.Set(min_uvx, min_uvy);
        Vertices[(2 * 4) + 1].UV.Set(max_uvx, max_uvy);
        Vertices[(2 * 4) + 2].UV.Set(min_uvx, max_uvy);
        Vertices[(2 * 4) + 3].UV.Set(max_uvx, min_uvy);
        Vertices[(2 * 4) + 0].Normal.Set(-1.0f, 0.0f, 0.0f);
        Vertices[(2 * 4) + 1].Normal.Set(-1.0f, 0.0f, 0.0f);
        Vertices[(2 * 4) + 2].Normal.Set(-1.0f, 0.0f, 0.0f);
        Vertices[(2 * 4) + 3].Normal.Set(-1.0f, 0.0f, 0.0f);
        Vertices[(3 * 4) + 0].Position.Set(max_x, min_y, max_z);
        Vertices[(3 * 4) + 1].Position.Set(max_x, max_y, min_z);
        Vertices[(3 * 4) + 2].Position.Set(max_x, max_y, max_z);
        Vertices[(3 * 4) + 3].Position.Set(max_x, min_y, min_z);
        Vertices[(3 * 4) + 0].UV.Set(min_uvx, min_uvy);
        Vertices[(3 * 4) + 1].UV.Set(max_uvx, max_uvy);
        Vertices[(3 * 4) + 2].UV.Set(min_uvx, max_uvy);
        Vertices[(3 * 4) + 3].UV.Set(max_uvx, min_uvy);
        Vertices[(3 * 4) + 0].Normal.Set(1.0f, 0.0f, 0.0f);
        Vertices[(3 * 4) + 1].Normal.Set(1.0f, 0.0f, 0.0f);
        Vertices[(3 * 4) + 2].Normal.Set(1.0f, 0.0f, 0.0f);
        Vertices[(3 * 4) + 3].Normal.Set(1.0f, 0.0f, 0.0f);
        Vertices[(4 * 4) + 0].Position.Set(max_x, min_y, max_z);
        Vertices[(4 * 4) + 1].Position.Set(min_x, min_y, min_z);
        Vertices[(4 * 4) + 2].Position.Set(max_x, min_y, min_z);
        Vertices[(4 * 4) + 3].Position.Set(min_x, min_y, max_z);
        Vertices[(4 * 4) + 0].UV.Set(min_uvx, min_uvy);
        Vertices[(4 * 4) + 1].UV.Set(max_uvx, max_uvy);
        Vertices[(4 * 4) + 2].UV.Set(min_uvx, max_uvy);
        Vertices[(4 * 4) + 3].UV.Set(max_uvx, min_uvy);
        Vertices[(4 * 4) + 0].Normal.Set(0.0f, -1.0f, 0.0f);
        Vertices[(4 * 4) + 1].Normal.Set(0.0f, -1.0f, 0.0f);
        Vertices[(4 * 4) + 2].Normal.Set(0.0f, -1.0f, 0.0f);
        Vertices[(4 * 4) + 3].Normal.Set(0.0f, -1.0f, 0.0f);
        Vertices[(5 * 4) + 0].Position.Set(min_x, max_y, max_z);
        Vertices[(5 * 4) + 1].Position.Set(max_x, max_y, min_z);
        Vertices[(5 * 4) + 2].Position.Set(min_x, max_y, min_z);
        Vertices[(5 * 4) + 3].Position.Set(max_x, max_y, max_z);
        Vertices[(5 * 4) + 0].UV.Set(min_uvx, min_uvy);
        Vertices[(5 * 4) + 1].UV.Set(max_uvx, max_uvy);
        Vertices[(5 * 4) + 2].UV.Set(min_uvx, max_uvy);
        Vertices[(5 * 4) + 3].UV.Set(max_uvx, min_uvy);
        Vertices[(5 * 4) + 0].Normal.Set(0.0f, 1.0f, 0.0f);
        Vertices[(5 * 4) + 1].Normal.Set(0.0f, 1.0f, 0.0f);
        Vertices[(5 * 4) + 2].Normal.Set(0.0f, 1.0f, 0.0f);
        Vertices[(5 * 4) + 3].Normal.Set(0.0f, 1.0f, 0.0f);

        for (CeU32 i = 0; i < 6; ++i)
        {
            CeU32 v_offset = i * 4;
            CeU32 i_offset = i * 6;
            Indices[i_offset + 0] = v_offset + 0;
            Indices[i_offset + 1] = v_offset + 1;
            Indices[i_offset + 2] = v_offset + 2;
            Indices[i_offset + 3] = v_offset + 0;
            Indices[i_offset + 4] = v_offset + 3;
            Indices[i_offset + 5] = v_offset + 1;
        }

        type = Box;
    }

    BoxGeometry::~BoxGeometry()
    {
        Indices.clear();
        Vertices.clear();
    }
}