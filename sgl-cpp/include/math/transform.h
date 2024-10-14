#pragma once

#include "math/matrix.h"
#include "math/vector.h"

namespace sgl
{

    constexpr mat4 translate(vec3 direction);
    constexpr mat4 translate(float dx, float dy, float dz);

    constexpr mat4 scale(vec3 scale);
    constexpr mat4 scale(float scaleX, float scaleY, float scaleZ);

    constexpr mat4 rotate(float angleX, float angleY, float angleZ);
    constexpr mat4 rotate(vec3 axis, float angle);

    constexpr mat4 ortho(float left, float right, float bottom, float top, float near, float far);
    constexpr mat4 perspective(float left, float right, float bottom, float top, float near, float far);
    constexpr mat4 viewport(int x, int y, int width, int height);

    inline constexpr mat4 translate(vec3 direction)
    {
        mat4 ret = mat4::identity;
        vec4 translate(direction, 1);
        ret[3] = translate;
        return ret;
    }

    inline constexpr mat4 translate(float dx, float dy, float dz)
    {
        mat4 ret = mat4::identity;
        vec4 translate(dx, dy, dz, 1);
        ret[3] = translate;
        return ret;
    }

    inline constexpr mat4 scale(vec3 scale)
    {
        mat4 ret = mat4::identity;
        ret[0][0] = scale[0];
        ret[1][1] = scale[1];
        ret[2][2] = scale[2];
        return ret;
    }

    inline constexpr mat4 scale(float scaleX, float scaleY, float scaleZ)
    {
        mat4 ret = mat4::identity;
        ret[0][0] = scaleX;
        ret[1][1] = scaleY;
        ret[2][2] = scaleZ;
        return ret;
    }

    inline constexpr mat4 rotate(float angleX, float angleY, float angleZ)
    {
        return mat4::identity;
    }

    inline constexpr mat4 rotate(vec3 axis, float angle)
    {
        return mat4::identity;
    }



}