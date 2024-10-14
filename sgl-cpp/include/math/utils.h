#pragma once

#include "math/vector.h"

#include <type_traits>

namespace sgl 
{

namespace math 
{

    template <size_t N, typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    T dotProduct(const Vector<N, T>& v1, const Vector<N, T>& v2)
    {
        T ret = static_cast<T>(0);
        for (int i = 0; i < N; ++i)
        {
            ret += v1.m_data[i] * v2.m_data[i];
        }
        return ret;
    }

}

}