#pragma once

#include "math/vector.h"

namespace sgl 
{

struct Material
{
    Material() = default;

    Material(const vec3& color, float kd, float ks, float shine, float T, float ior) :
        color(color), kd(kd), ks(ks), shine(shine), T(T), ior(ior)
    {
    }

    vec3 color; // { r, g, b }
    float kd;
    float ks;
    float shine;
    float T;
    float ior;
};

}
