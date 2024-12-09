#pragma once

#include "math/vector.h"

namespace sgl
{

    struct Ray
    {
        enum class Type
        {
            NORMAL,
            INSIDE
        };

        const static unsigned MAX_DEPTH = 8;

        Ray(const Ray&) = default;
        Ray(Ray&&) = default;
        Ray() = delete;

        Ray(const vec3& origin, const vec3& dir, Type type = Ray::Type::NORMAL);
        Ray(float ox, float oy, float oz, float dx, float dy, float dz, Type type = Ray::Type::NORMAL);

        vec3 origin;
        vec3 dir;
        Type type;
    };

}

