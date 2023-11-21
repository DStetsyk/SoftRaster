#pragma once
#include <glm/glm.hpp>

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Vec2i = glm::ivec2;

struct Color
{
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 0;

    Color operator*(float f)
    {
        return {
            (unsigned char)(f * r),
            (unsigned char)(f * g),
            (unsigned char)(f * b),
            (unsigned char)(f * a)
        };
    }
};

Color operator+ (const Color& c0, const Color& c1);

template <typename T>
T linearInterp(T& start, T& end, float t)
{
    T ret = start * (1.0f - t) + end * t;
    return ret;
}

