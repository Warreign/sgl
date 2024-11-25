#pragma once

#include "math/vector.h"

namespace sgl
{

class Light
{
public:
    Light() = default;
    Light(Light&&) = default;
    Light(const Light&) = default;
};

class PointLight : public Light
{
public:
    PointLight(PointLight&&) = default;

    PointLight(const vec3& pos, const vec3& color);

private:
    vec3 m_pos;
    vec3 m_color;
};

}