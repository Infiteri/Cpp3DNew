#pragma once

#include "Base.h"
#include "Math/Vectors.h"
#include <vector>

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
        int LatitudeSegments = 16;
        int LongitudeSegments = 16;

        SphereGeometry(float radius = 1, int latitudeSegments = 16, int longitudeSegments = 16);
        ~SphereGeometry();
    };
}