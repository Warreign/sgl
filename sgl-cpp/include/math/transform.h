#pragma once

#include "math/matrix.h"
#include "math/vector.h"

namespace sgl
{

    mat4 translate(const vec3& direction);
    mat4 translate(float dx, float dy, float dz);

    mat4 scale(const vec3& scale);
    mat4 scale(float scaleX, float scaleY, float scaleZ);
    
    mat4 rotateZ(float angle);
    mat4 rotateY(float angle);
    mat4 rotate(const vec3& axis, float angle);
    
    mat4 ortho(float left, float right, float bottom, float top, float near, float far);
    mat4 perspective(float left, float right, float bottom, float top, float near, float far);
    mat4 viewport(int x, int y, int width, int height);
}