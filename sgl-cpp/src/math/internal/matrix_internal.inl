#include "matrix_internal.h"
#include <cassert>
#include <algorithm>
#include <numeric>

#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wc++20-extensions"

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
    inline Matrix<M, N, T> Matrix<N, M, T>::transpose() const
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
    template <size_t _N, size_t _M, typename _T, typename >
    constexpr Matrix<_N, _M, _T> Matrix<N, M, T>::inverse() const
    {
        Matrix<N, M, T> ret = *this;

        int indxc[4],indxr[4],ipiv[4];
        int i,icol,irow,j,k,l,ll,n;
        float big,dum,pivinv,temp;
        // satisfy the compiler
        icol = irow = 0;
            
        // the size of the matrix
        n = 4;
            
        for ( j = 0 ; j < n ; j++) /* zero pivots */
        ipiv[j] = 0;
            
        for ( i = 0; i < n; i++)
        {
            big = 0.0;
            for (j = 0 ; j < n ; j++)
            if (ipiv[j] != 1)
                for ( k = 0 ; k<n ; k++)
                {
                    if (ipiv[k] == 0)
                    {
                        if (std::abs(ret[k][j]) >= big)
                        {
                            big = std::abs(ret[k][j]);
                            irow = j;
                            icol = k;
                        }
                    }
                }
            ++(ipiv[icol]);
            if (irow != icol)
            {
                for ( l = 0 ; l<n ; l++)
                {
                    temp = ret[l][icol];
                    ret[l][icol] = ret[l][irow];
                    ret[l][irow] = temp;
                }
            }
            indxr[i] = irow;
            indxc[i] = icol;
                    
            pivinv = 1.0 / ret[icol][icol];
            ret[icol][icol] = 1.0 ;
            for ( l = 0 ; l<n ; l++)
            ret[l][icol] = ret[l][icol] * pivinv ;
                    
            for (ll = 0 ; ll < n ; ll++)
            if (ll != icol)
                {
                dum = ret[icol][ll];
                ret[icol][ll] = 0.0;
                for ( l = 0 ; l<n ; l++)
                    ret[l][ll] = ret[l][ll] - ret[l][icol] * dum ;
                }
        }
        for ( l = n; l--; )
        {
            if (indxr[l] != indxc[l])
            for ( k = 0; k<n ; k++)
                {
                temp = ret[indxr[l]][k];
                ret[indxr[l]][k] = ret[indxc[l]][k];
                ret[indxc[l]][k] = temp;
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

#ifdef SGL_SIMD
       
    template<>
    inline Matrix<4, 4, float> Matrix<4, 4, float>::transpose() const
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

    inline Matrix<4, 4, float> operator+(const Matrix<4, 4, float>& m1, const Matrix<4, 4, float>& m2)
    {
        Matrix<4,4,float> result;

        for (int i = 0; i < 4; ++i)
        {
            __m128 col1 = _mm_loadu_ps(m1[i].data_ptr());
            __m128 col2 = _mm_loadu_ps(m2[i].data_ptr());

            __m128 rescol = _mm_add_ps(col1, col2);

            _mm_storeu_ps(result[i].data_ptr(), rescol);
        }
        return result;
    }

    inline Matrix<4, 4, float> operator-(const Matrix<4, 4, float>& m1, const Matrix<4, 4, float>& m2)
    {
        Matrix<4,4,float> result;

        for (int i = 0; i < 4; ++i)
        {
            __m128 col1 = _mm_loadu_ps(m1[i].data_ptr());
            __m128 col2 = _mm_loadu_ps(m2[i].data_ptr());

            __m128 rescol = _mm_sub_ps(col1, col2);

            _mm_storeu_ps(result[i].data_ptr(), rescol);
        }
        return result;
    }

    inline Matrix<4, 4, float> operator*(const Matrix<4, 4, float>& m1, const Matrix<4, 4, float>& m2)
    {
        Matrix<4,4, float> result;

        __m128 rows[4] { _mm_loadu_ps(m1[0].data_ptr()),
                         _mm_loadu_ps(m1[1].data_ptr()),
                         _mm_loadu_ps(m1[2].data_ptr()),
                         _mm_loadu_ps(m1[3].data_ptr()) };

        _MM_TRANSPOSE4_PS(rows[0], rows[1], rows[2], rows[3]);

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                __m128 row = rows[i];
                __m128 col = _mm_loadu_ps(m2[j].data_ptr());

                __m128 mul = _mm_mul_ps(row, col);

                __m128 sum = _mm_hadd_ps(mul, mul);
                sum = _mm_hadd_ps(sum, sum);

                result[i][j] = _mm_cvtss_f32(sum);
            }
        }

        // _MM_TRANSPOSE4_PS(rows[0], rows[1], rows[2], rows[3]);

        // _mm_storeu_ps(result[0].data_ptr(), rows[0]);
        // _mm_storeu_ps(result[1].data_ptr(), rows[1]);
        // _mm_storeu_ps(result[2].data_ptr(), rows[2]);
        // _mm_storeu_ps(result[3].data_ptr(), rows[3]);

        return result.transpose();
    }

    inline Vector<4, float> operator*(const Matrix<4, 4, float>& m, const Vector<4, float>& v)
    {
        Vector<4, float> ret;

        __m128 rows[4] { _mm_loadu_ps(m[0].data_ptr()),
                         _mm_loadu_ps(m[1].data_ptr()),
                         _mm_loadu_ps(m[2].data_ptr()),
                         _mm_loadu_ps(m[3].data_ptr()) };

        _MM_TRANSPOSE4_PS(rows[0], rows[1], rows[2], rows[3]);

        __m128 vc = _mm_loadu_ps(v.data_ptr());
            
        for (int i = 0; i < 4; ++i)
        {
            __m128 mul = _mm_mul_ps(rows[i], vc);
            __m128 sum = _mm_hadd_ps(mul, mul);
            sum = _mm_hadd_ps(sum, sum);

            ret[i] = _mm_cvtss_f32(sum);
        }

        // _mm_storeu_ps(ret.data_ptr(), res);

        return ret;
    }
#endif

}