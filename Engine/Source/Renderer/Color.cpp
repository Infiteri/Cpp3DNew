#include "Color.h"
namespace Core
{
    Color::Color(float r, float g, float b, float a)
    {
    }

    Color::~Color()
    {
    }

    void Color::Set(const Color &color)
    {
        Set(color.r, color.g, color.b, color.a);
    }

    void Color::Set(Color *color)
    {
        Set(color->r, color->g, color->b, color->a);
    }

    void Color::Set(float r, float g, float b, float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

}