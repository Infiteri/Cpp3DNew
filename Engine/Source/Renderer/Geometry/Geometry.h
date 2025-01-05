#pragma once

#include "Base.h"
#include "Math/Vectors.h"
#include "Math/Math.h"

#include <vector>
#include <unordered_map>
#include <cmath>

namespace Core
{
    struct Vertex3D
    {
        Vector3 Position;
        Vector2 UV;
        Vector3 Normal;
    };

    class Geometry
    {

    public:
        enum Types
        {
            None,
            Box,
            Sphere
        };

    protected:
        Types type;

    public:
        std::vector<Vertex3D> Vertices;
        std::vector<CeU32> Indices;

        inline Types GetType() { return type; };

        Geometry() { type = None; };
        virtual ~Geometry() {};

        template <typename T>
        T *As() { return (T *)this; };
    };

    class CE_API BoxGeometry : public Geometry
    {
    public:
        float Width = 1;
        float Height = 1;
        float Depth = 1;

        BoxGeometry(float width = 1, float height = 1, float depth = 1);
        ~BoxGeometry();
    };

    class CE_API SphereGeometry : public Geometry
    {
    public:
        float Radius = 1;
        int Subdivision = 1;

        SphereGeometry(float radius = 1, int subdivision = 1);
        ~SphereGeometry();

    private:
        void Subdivide()
        {
            std::unordered_map<CeU64, CeU32> midPointCache;
            std::vector<CeU32> newIndices;

            auto getMidpoint = [&](CeU32 v1, CeU32 v2) -> CeU32
            {
                CeU64 key = (CeU64)std::min(v1, v2) << 32 | std::max(v1, v2);
                if (midPointCache.find(key) != midPointCache.end())
                    return midPointCache[key];

                const Vector3 &p1 = Vertices[v1].Position;
                const Vector3 &p2 = Vertices[v2].Position;

                Vector3 mid = (p1 + p2) * 0.5f;
                mid.Normalize();
                CeU32 index = static_cast<CeU32>(Vertices.size());

                Vertex3D v;
                v.Position = mid * Radius;
                v.Normal = mid;
                v.UV = {0, 0};
                Vertices.push_back(v); // UVs will be updated later

                midPointCache[key] = index;
                return index;
            };

            for (size_t i = 0; i < Indices.size(); i += 3)
            {
                CeU32 v1 = Indices[i];
                CeU32 v2 = Indices[i + 1];
                CeU32 v3 = Indices[i + 2];

                CeU32 a = getMidpoint(v1, v2);
                CeU32 b = getMidpoint(v2, v3);
                CeU32 c = getMidpoint(v3, v1);

                newIndices.insert(newIndices.end(), {v1, a, c});
                newIndices.insert(newIndices.end(), {v2, b, a});
                newIndices.insert(newIndices.end(), {v3, c, b});
                newIndices.insert(newIndices.end(), {a, b, c});
            }

            Indices = std::move(newIndices);
        }

        void ComputeUV(const Vector3 &pos, Vector2 &uv)
        {
            float u = 0.5f + atan2(pos.z, pos.x) / (2 * CE_PI);
            float v = 0.5f - asin(pos.y / Radius) / CE_PI;
            uv = {u, v};
        }
    };
}