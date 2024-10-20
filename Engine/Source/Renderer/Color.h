#pragma once

#include "Base.h"

namespace Core
{
    class CE_API Color
    {
    public:
        float r = 255;
        float g = 255;
        float b = 255;
        float a = 255;

        Color(float r = 255, float g = 255, float b = 255, float a = 255);
        ~Color();

        inline Color Normalized() { return {r / 255, g / 255, b / 255, a / 255}; };

        void Set(const Color &color);
        void Set(Color *color);
        void Set(float r, float g, float b, float a);
    };

}