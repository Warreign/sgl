#include "matrix_internal.h"
#include <cassert>
#include <algorithm>
#include <numeric>

namespace sgl
{
    template <size_t N, size_t M, typename T>
    inline constexpr Matrix<N, M, T>::Matrix(const column_t (& list)[N])
    {
        for (int i = 0; i < N; ++i)
        {
            m_data[i] = list[i];
        }
    }

    template <size_t N, size_t M, typename T>
    inline constexpr Matrix<N, M, T>::Matrix(const T (& list)[N * M])
    {
        for (size_t m = 0; m < M; ++m)
        {
            for (size_t n = 0; n < N; ++n)
            {
                m_data[n][m] = list[m * N + n];
            }
        }
    }

    template <size_t N, size_t M, typename T>
    template <size_t , size_t , typename >
    inline constexpr Matrix<N, M, T> Matrix<N, M, T>::_genIdentity()
    {
        column_t cols[N];
        for (int i = 0; i < N; ++i)
        {
            Vector<N, T> v;
            v[i] = 1;
            cols[i] = v;
        }
        return Matrix<N, N, T>(cols);
    }

    template <size_t N, size_t M, typename T>
    inline constexpr Matrix<M, N, T> Matrix<N, M, T>::transpose() const
    {
        Matrix<M, N, T> ret;
        for (size_t n = 0; n < N; ++n)
        {
            for (size_t m = 0; m < M; ++m)
            {
                ret[m][n] = m_data[n][m];
            }
        }
        return ret;
    }

    template <size_t N, size_t M, typename T>
    inline constexpr Matrix<N, M, T>& Matrix<N, M, T>::operator+=(const Matrix<N, M, T> &other)
    {
        for (size_t i = 0; i < N; ++i)
        {
            m_data[i] += other.m_data[i];
        }
        return *this;
    }

    template <size_t N, size_t M, typename T>
    inline constexpr Matrix<N, M, T>& Matrix<N, M, T>::operator-=(const Matrix<N, M, T> &other)
    {
        for (size_t i = 0; i < N; ++i)
        {
            m_data[i] -= other.m_data[i];
        }
        return *this;
    }

/*
    template <size_t N, size_t M, typename T>
    inline constexpr Matrix<N, M, T>& Matrix<N, M, T>::operator*=(const Matrix<M, N, T> &other)
    {
        for (size_t i = 0; i < N; ++i)
        {
            m_data[i] += other.m_data[i];
        }
    }
*/

    template <size_t N, size_t M, typename T>
    inline constexpr const T* Matrix<N, M, T>::data_ptr() const
    {
        return m_data[0].data_ptr();
    }

    template <size_t N, size_t M, typename T>
    inline constexpr typename Matrix<N, M, T>::column_t& Matrix<N, M, T>::operator[](size_t i)
    {
        assert(i >= 0 && i < N);
        return m_data[i];
    }

    template <size_t N, size_t M, typename T>
    inline constexpr const typename Matrix<N, M, T>::column_t& Matrix<N, M, T>::operator[](size_t i) const
    {
        assert(i >= 0 && i < N);
        return m_data[i];
    }

    template <size_t N, size_t M, typename T>
    inline constexpr Matrix<N, M, T>& Matrix<N, M, T>::operator=(const Matrix<N, M, T> &other) 
    {
        m_data = other.m_data;
        return *this;
    }

    template <size_t N, size_t M, typename T>
    inline constexpr Matrix<N, M, T> operator+(const Matrix<N, M, T> &m1, const Matrix<N, M, T> &m2)
    {
        Matrix<N, M, T> ret = m1;
        ret += m2;
        return ret;
    }

    template <size_t N, size_t M, typename T>
    inline constexpr Matrix<N, M, T> operator-(const Matrix<N, M, T> &m1, const Matrix<N, M, T> &m2)
    {
        Matrix<N, M, T> ret = m1;
        ret -= m2;
        return ret;
    }

    template <size_t N, size_t M, size_t M2, typename T>
    inline constexpr Matrix<M2, M, T> operator*(const Matrix<N, M, T> &m1, const Matrix<M2, N, T> &m2) 
    {
        Matrix<M2, M, T> ret;
        Matrix<N, M, T> _m1 = m1.transpose();

        for (size_t i = 0; i < M; ++i)
        {
            for (size_t j = 0; j < M2; ++j)
            {
                auto v = _m1[i];
                auto& v2 = m2[j];
                T val = static_cast<T>(0.0);
                v *= v2;
                val = std::reduce(v.m_data.begin(), v.m_data.end(), static_cast<T>(0.0));
                ret[j][i] = val;
            }
        }

        return ret;
    }

    template <size_t N, size_t M, typename T>
    inline constexpr Vector<M, T> operator*(const Matrix<N, M, T>& m, const Vector<M, T>& v)
    {
        Matrix<1, M, T> mv({v});
        return (m * mv)[0];
    }

    template <size_t N, size_t M, typename T>
    inline const Matrix<N, M, T> Matrix<N, M, T>::identity = Matrix<N, M, T>::_genIdentity();

    template <size_t N, size_t M, typename T>
    template <typename... U, typename >
    inline constexpr Matrix<N, M, T>::Matrix(const U& ... values)
    {
        int i = 0;
        ([&] {
            int n = i % N;
            int m = i / N;
            this->m_data[n][m] = static_cast<T>(values);
            ++i;
        } (), ...);
    }

    template <size_t N, size_t M, typename T>
    inline constexpr Matrix<N, M, T>& Matrix<N, M, T>::operator*=(const Matrix<M, N, T>& other)
    {
        *this = other * *this;
        return *this;
    }

    template <size_t N, size_t M, typename T>
    inline constexpr Matrix<N, M, T>::Matrix(const T* ptr)
    {
        for (int n = 0; n < N; ++n)
        {
            for (int m = 0; m < M; ++m)
            {
                m_data[n][m] = ptr[n * M + m];
            }
        }
    }

    // SIMD 
    
    inline constexpr Matrix<4, 4, float> operator+(const Matrix<4, 4, float>& m1, const Matrix<4, 4, float>& m2)
    {
        __m256 col01in1 = _mm256_loadu_ps(m1[0].data_ptr());
        __m256 col23in1 = _mm256_loadu_ps(m1[2].data_ptr());

        __m256 col01in2 = _mm256_loadu_ps(m2[0].data_ptr());
        __m256 col23in2 = _mm256_loadu_ps(m2[2].data_ptr());

        __m256 col01out = _mm256_add_ps(col01in1, col01in2);
        __m256 col23out = _mm256_add_ps(col23in1, col23in2);

        Matrix<4,4,float> result;
        _mm256_storeu_ps(result[0].data_ptr(), col01out);
        _mm256_storeu_ps(result[2].data_ptr(), col23out);

        return result;
    }

    inline constexpr Matrix<4, 4, float> operator-(const Matrix<4, 4, float>& m1, const Matrix<4, 4, float>& m2)
    {
        __m256 col01in1 = _mm256_loadu_ps(m1[0].data_ptr());
        __m256 col23in1 = _mm256_loadu_ps(m1[2].data_ptr());

        __m256 col01in2 = _mm256_loadu_ps(m2[0].data_ptr());
        __m256 col23in2 = _mm256_loadu_ps(m2[2].data_ptr());

        __m256 col01out = _mm256_sub_ps(col01in1, col01in2);
        __m256 col23out = _mm256_sub_ps(col23in1, col23in2);

        Matrix<4,4,float> result;
        _mm256_storeu_ps(result[0].data_ptr(), col01out);
        _mm256_storeu_ps(result[2].data_ptr(), col23out);

        return result;
    }

    inline constexpr Matrix<4, 4, float> operator*(const Matrix<4, 4, float>& m1, const Matrix<4, 4, float>& m2)
    {
        Matrix<4,4, float> result;

        __m128 rows[4];
        rows[0] = _mm_loadu_ps(m1[0].data_ptr());
        rows[1] = _mm_loadu_ps(m1[1].data_ptr());
        rows[2] = _mm_loadu_ps(m1[2].data_ptr());
        rows[3] = _mm_loadu_ps(m1[3].data_ptr());

        _MM_TRANSPOSE4_PS(rows[0], rows[1], rows[2], rows[3]);

        __m256 row01m2 = _mm256_loadu_ps(m2[0].data_ptr());
        __m256 row23m2 = _mm256_loadu_ps(m2[2].data_ptr());

        for (int i = 0; i < 4; ++i)
        {
            __m256 rowm1 = _mm256_set_m128(rows[i], rows[i]);
            
            __m256 val0 = _mm256_mul_ps(rowm1, row01m2);
            val0 = _mm256_hadd_ps(val0, val0);
            val0 = _mm256_hadd_ps(val0, val0);

            __m256 val1 = _mm256_mul_ps(rowm1, row23m2);
            val1 = _mm256_hadd_ps(val1, val1);
            val1 = _mm256_hadd_ps(val1, val1);

            __m256i idxMask = _mm256_set_epi32(4, 0, 4, 0, 4, 0, 4, 0);
            __m256 resperm01 = _mm256_permutevar8x32_ps(val0, idxMask);
            __m256 resperm23 = _mm256_permutevar8x32_ps(val1, idxMask);
            __m128 res01 = _mm256_castps256_ps128(resperm01);
            __m128 res23 = _mm256_castps256_ps128(resperm23);

            __m128 res = _mm_movelh_ps(res01, res23);

            rows[i] = res;
        }

        _MM_TRANSPOSE4_PS(rows[0], rows[1], rows[2], rows[3]);

        _mm_storeu_ps(result[0].data_ptr(), rows[0]);
        _mm_storeu_ps(result[1].data_ptr(), rows[1]);
        _mm_storeu_ps(result[2].data_ptr(), rows[2]);
        _mm_storeu_ps(result[3].data_ptr(), rows[3]);

        return result;
    }
    
    template<>
    inline constexpr Matrix<4, 4, float> Matrix<4, 4, float>::transpose() const
    {
        Matrix<4, 4, float> ret;

        __m128 row0 = _mm_loadu_ps(m_data[0].data_ptr());
        __m128 row1 = _mm_loadu_ps(m_data[1].data_ptr());
        __m128 row2 = _mm_loadu_ps(m_data[2].data_ptr());
        __m128 row3 = _mm_loadu_ps(m_data[3].data_ptr());

        _MM_TRANSPOSE4_PS(row0, row1, row2, row3);

        _mm_storeu_ps(ret[0].data_ptr(), row0);
        _mm_storeu_ps(ret[1].data_ptr(), row1);
        _mm_storeu_ps(ret[2].data_ptr(), row2);
        _mm_storeu_ps(ret[3].data_ptr(), row3);

        return ret;
    }

    inline constexpr Vector<4, float> operator*(const Matrix<4, 4, float>& m, const Vector<4, float>& v)
    {
        __m128 rows[4];
        rows[0] = _mm_loadu_ps(m[0].data_ptr());
        rows[1] = _mm_loadu_ps(m[1].data_ptr());
        rows[2] = _mm_loadu_ps(m[2].data_ptr());
        rows[3] = _mm_loadu_ps(m[3].data_ptr());

        _MM_TRANSPOSE4_PS(rows[0], rows[1], rows[2], rows[3]);

        __m256 row01m2 = _mm256_setr_m128(rows[0], rows[1]);
        __m256 row23m2 = _mm256_setr_m128(rows[2], rows[3]);

        __m128 vc128 = _mm_loadu_ps(v.data_ptr());
        __m256 vc = _mm256_broadcast_ps(&vc128);
            
        __m256 val0 = _mm256_mul_ps(vc, row01m2);
        val0 = _mm256_hadd_ps(val0, val0);
        val0 = _mm256_hadd_ps(val0, val0);

        __m256 val1 = _mm256_mul_ps(vc, row23m2);
        val1 = _mm256_hadd_ps(val1, val1);
        val1 = _mm256_hadd_ps(val1, val1);

        __m256i idxMask = _mm256_set_epi32(4, 0, 4, 0, 4, 0, 4, 0);
        __m256 resperm01 = _mm256_permutevar8x32_ps(val0, idxMask);
        __m256 resperm23 = _mm256_permutevar8x32_ps(val1, idxMask);
        __m128 res01 = _mm256_castps256_ps128(resperm01);
        __m128 res23 = _mm256_castps256_ps128(resperm23);

        __m128 res = _mm_movelh_ps(res01, res23);

        Vector<4, float> ret;
        _mm_storeu_ps(ret.data_ptr(), res);

        return ret;
    }

}