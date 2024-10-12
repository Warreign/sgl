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
        return m_data.data();
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

}