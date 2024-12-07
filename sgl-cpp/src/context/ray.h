#pragma once

#include "math/vector.h"

namespace sgl
{

    struct Ray
    {
        const static unsigned MAX_DEPTH = 8;

        Ray(const Ray&) = default;
        Ray(Ray&&) = default;
        Ray() = delete;

        Ray(const vec3& origin, const vec3& dir);
        Ray(float ox, float oy, float oz, float dx, float dy, float dz);

        vec3 origin;
        vec3 dir;
    };

}

