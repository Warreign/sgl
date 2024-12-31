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

    virtual bool isEmissive() const { return false; };
    virtual ~Material() = default;

    vec3 color; // { r, g, b }
    float kd;
    float ks;
    float shine;
    float T;
    float ior;
};

struct EmissiveMaterial : public Material
{
    EmissiveMaterial(const vec3& color, const float c0, const float c1, const float c2) :
        Material(color, 1, 0, 0, 0, 1), c0(c0), c1(c1), c2(c2)
    {
    }

    virtual bool isEmissive() const { return true; }

    float c0, c1, c2;
};

}