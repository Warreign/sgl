#pragma once

#include "internal/matrix_internal.h"

namespace sgl 
{

    // Matrix definitions
    using mat2x1 = Matrix<2, 1, float>;
    using mat2x2 = Matrix<2, 2, float>;
    using mat1x2 = Matrix<1, 2, float>;
    using mat2 = mat2x2;

    using mat3x1 = Matrix<3, 1, float>;
    using mat3x2 = Matrix<3, 2, float>;
    using mat3x3 = Matrix<3, 3, float>;
    using mat2x3 = Matrix<3, 3, float>;
    using mat1x3 = Matrix<3, 3, float>;
    using mat3 = mat3x3;

    using mat4x1 = Matrix<4, 1, float>;
    using mat4x2 = Matrix<4, 2, float>;
    using mat4x3 = Matrix<4, 3, float>;
    using mat4x4 = Matrix<4, 4, float>;
    using mat3x4 = Matrix<3, 4, float>;
    using mat2x4 = Matrix<2, 4, float>;
    using mat1x4 = Matrix<1, 4, float>;
    using mat4 = mat4x4;

}