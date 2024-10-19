#pragma once

#include "internal/vector_internal.h"

#include <cmath>

namespace sgl 
{

namespace math 
{

    template <size_t N, typename T>
    constexpr T dotProduct(const Vector<N, T>& v1, const Vector<N, T>& v2)
    {
        T ret = static_cast<T>(0);
        for (int i = 0; i < N; ++i)
        {
            ret += v1.m_data[i] * v2.m_data[i];
        }
        return ret;
    }

    template <size_t N, typename T>
    constexpr T distance(const Vector<N, T>& p1, const Vector<N, T>& p2)
    {
        Vector<N, T> sub = p2 - p1;
        return std::sqrt(dotProduct(sub, sub));
    }

}

}