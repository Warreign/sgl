#pragma once

#include "math/matrix.h"
#include "math/vector.h"

namespace sgl
{

    constexpr mat4 translate(const vec3& direction)
    {
        mat4 ret = mat4::identity;
        vec4 translate(direction, 1);
        ret[3] = translate;
        return ret;
    }

    constexpr mat4 translate(float dx, float dy, float dz)
    {
        mat4 ret = mat4::identity;
        vec4 translate(dx, dy, dz, 1);
        ret[3] = translate;
        return ret;
    }

    constexpr mat4 scale(const vec3& scale)
    {
        mat4 ret = mat4::identity;
        ret[0][0] = scale[0];
        ret[1][1] = scale[1];
        ret[2][2] = scale[2];
        return ret;
    }

    constexpr mat4 scale(float scaleX, float scaleY, float scaleZ)
    {
        mat4 ret = mat4::identity;
        ret[0][0] = scaleX;
        ret[1][1] = scaleY;
        ret[2][2] = scaleZ;
        return ret;
    }

    constexpr mat4 rotateZ(float angle)
    {
        return mat4(std::cos(angle), -std::sin(angle), 0, 0,
                    std::sin(angle),  std::cos(angle), 0, 0,
                    0,                    0,                  1, 0,
                    0,                    0,                  0, 1);
    }

    constexpr mat4 rotateY(float angle)
    {
        return mat4(std::cos(angle), 0, -std::sin(angle), 0,
                    0,                   1, 0,                   0,
                    std::sin(angle), 0,  std::cos(angle), 0,
                    0,                   0, 0,                   0);
    }

    constexpr mat4 rotate(const vec3& axis, float angle)
    {
        return mat4::identity;
    }

    constexpr mat4 ortho(float left, float right, float bottom, float top, float near, float far) 
    {
        return mat4(2/(right-left), 0, 0, -(right+left)/(right-left),
                    0, 2/(top-bottom), 0, -(top+bottom)/(top-bottom),
                    0, 0, -2/(far-near), -(far+near)/(far-near),
                    0, 0, 0, 1);
    }

    constexpr mat4 perspective(float left, float right, float bottom, float top, float near, float far)
    {
        return mat4::identity;
    }

    constexpr mat4 viewport(int x, int y, int width, int height)
    {
        return mat4::identity;
    }
}