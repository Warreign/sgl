#pragma once

#include "math/vector.h"

namespace sgl
{

struct Ray
{
    Ray(const Ray&) = default;
    Ray(Ray&&) = default;
    Ray() = delete;

    Ray(const vec3& dir);
    Ray(float x, float y, float z);

    vec3 dir;
};

}
