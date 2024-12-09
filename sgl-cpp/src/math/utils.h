#pragma once

#include "math/internal/vector_internal.h"

#include <cmath>
#include <type_traits>

#ifdef SGL_SIMD
#include <immintrin.h>
#include <pmmintrin.h>
#pragma GCC target("sse3")
#pragma GCC target("avx")
#endif


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

        template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr Vector<3, T> crossProduct(const Vector<3, T>& v1, const Vector<3, T>& v2)
        {
            Vector<3, T> result;
            // result[0] = v1[1] * v2[2] - v1[2] * v2[1];
            // result[1] = v1[2] * v2[0] - v1[0] * v2[2];
            // result[2] = v1[0] * v2[1] - v1[1] * v2[0];
            result.x = v1.y*v2.z - v1.z*v2.y;
            result.y = v1.z*v2.x - v1.x*v2.z;
            result.z = v1.x*v2.y - v1.y*v2.x;
            return result;
        }

#ifdef SGL_SIMD
        template <size_t N, typename = std::enable_if_t<N <= 4>>
        constexpr float dotProduct(const Vector<N, float> v1, const Vector<N, float>& v2)
        {
            Vector<4, float> v1full = v1;
            Vector<4, float> v2full = v2;

            __m128 a = _mm_loadu_ps(v1full.data_ptr());
            __m128 b = _mm_loadu_ps(v2full.data_ptr());

            __m128 mul = _mm_mul_ps(a, b);
            __m128 sum = _mm_hadd_ps(mul, mul);
            sum = _mm_hadd_ps(sum, sum);

            return _mm_cvtss_f32(sum);
        }
#endif

        template <size_t N, typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr T length(const Vector<N, T>& v)
        {
            return std::sqrt(dotProduct(v, v));
        }

        template <size_t N, typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr Vector<N, T> normalize(const Vector<N, T>& v)
        {
            return v / length(v);
        }

        
        template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr Vector<3, T> reflect(const Vector<3, T>& incoming, const Vector<3, T>& normal)
        {
            return incoming - 2  * math::dotProduct(incoming, normal) * normal;
        }

        template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr Vector<3, T> refract(const Vector<3, T>& incoming, const Vector<3, T>& normal, T coef)
        {
            float gamma = 0, sqrterm = 0;
            float dot = math::dotProduct(incoming, normal);
            gamma = 1.0 / coef;
            sqrterm = 1.0 - gamma * gamma * (1.0 - dot * dot);

            if (sqrterm > 0.0) {
                sqrterm = dot * gamma + sqrt(sqrterm);
                return math::normalize( -sqrterm * normal + incoming * gamma );
            }
            return vec3();
        }

        template <size_t N, typename T>
        constexpr T distance(const Vector<N, T>& p1, const Vector<N, T>& p2)
        {
            Vector<N, T> sub = p2 - p1;
            return std::sqrt(dotProduct(sub, sub));
        }

    }

    

}