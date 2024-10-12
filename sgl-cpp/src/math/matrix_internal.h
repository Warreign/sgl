#pragma once

#include "math/Vector.h"

#include <array>
#include <numeric>
#include <type_traits>
#include <algorithm>
#include <ostream>

namespace sgl 
{

    template <size_t N, size_t M, typename T>
    struct  Matrix
    {
        using column_t = Vector<M, T>;
        using row_t = Vector<N, T>;
        constexpr static std::size_t column_size = M;
        constexpr static std::size_t row_size = N;

        // Default constructor
        constexpr Matrix() = default;

        // Copy constructor
        constexpr Matrix(const Matrix<N, M, T>&) = default;

        // Move constructor
        constexpr Matrix(Matrix<N, M, T>&& other) = default;

        // Initializer list constructors
        constexpr Matrix(const column_t (& list)[N]);
        constexpr Matrix(const T (& list)[N * M]);

        // Addition assignment
        constexpr Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& other);
        constexpr Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& other);
        // constexpr Matrix<N, M, T>& operator*=(const Matrix<M, N, T>& other);

        // Access operators
        constexpr column_t& operator[](size_t i);
        constexpr const column_t& operator[](size_t i) const;
        
        // Assignment operators
        constexpr Matrix<N, M, T>& operator=(const Matrix<N, M, T>& other);

        constexpr Matrix<M, N, T> transpose() const;

        constexpr const T* data_ptr() const;

        friend std::ostream &operator<<(std::ostream &os, const Matrix<N, M, T> &rhs)
        {
            os << rhs.m_data[0];
            for (size_t i = 1; i < N; ++i)
            {
                os << '\n';
                os << rhs.m_data[i];
            }
            return os << '\n';
        }

        // Identity of current matrix (if exists)
        const static Matrix<N, M, T> identity;

        
    private:

        std::array<column_t, N> m_data;
    
        template <size_t _N = N, size_t _M = M, typename = std::enable_if_t<_N == _M>>
        constexpr static Matrix<N, M, T> _genIdentity();

    };

    template <size_t N, size_t M, typename T>
    constexpr Matrix<N, M, T> operator+(const Matrix<N, M, T>& m1, const Matrix<N, M, T>& m2);
    template <size_t N, size_t M, typename T>
    constexpr Matrix<N, M, T> operator-(const Matrix<N, M, T>& m1, const Matrix<N, M, T>& m2);
    template <size_t N, size_t M, size_t M2, typename T>
    constexpr Matrix<M2, M, T> operator*(const Matrix<N, M, T>& m1, const Matrix<M2, N, T>& m2);

    template <size_t N, size_t M, typename T>
    constexpr Matrix<N, M, T>::Matrix(const column_t (& list)[N])
    {
        for (int i = 0; i < N; ++i)
        {
            m_data[i] = list[i];
        }
    }

    template <size_t N, size_t M, typename T>
    constexpr Matrix<N, M, T>::Matrix(const T (& list)[N * M])
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
    constexpr Matrix<N, M, T> Matrix<N, M, T>::_genIdentity()
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
    constexpr Matrix<M, N, T> Matrix<N, M, T>::transpose() const
    {
        Matrix<M, N, T> ret;
        for (size_t n = 0; n < N; ++n)
        {
            for (size_t m = 0; m < M; ++m)
            {
                ret.m_data[n][m] = m_data[m][n];
            }
        }
        return ret;
    }

    template <size_t N, size_t M, typename T>
    constexpr Matrix<N, M, T>& Matrix<N, M, T>::operator+=(const Matrix<N, M, T> &other)
    {
        for (size_t i = 0; i < N; ++i)
        {
            m_data[i] += other.m_data[i];
        }
        return *this;
    }

    template <size_t N, size_t M, typename T>
    constexpr Matrix<N, M, T>& Matrix<N, M, T>::operator-=(const Matrix<N, M, T> &other)
    {
        for (size_t i = 0; i < N; ++i)
        {
            m_data[i] -= other.m_data[i];
        }
        return *this;
    }

/*
    template <size_t N, size_t M, typename T>
    constexpr Matrix<N, M, T>& Matrix<N, M, T>::operator*=(const Matrix<M, N, T> &other)
    {
        for (size_t i = 0; i < N; ++i)
        {
            m_data[i] += other.m_data[i];
        }
    }
*/

    template <size_t N, size_t M, typename T>
    constexpr const T* Matrix<N, M, T>::data_ptr() const
    {
        return m_data.data();
    }

    template <size_t N, size_t M, typename T>
    constexpr typename Matrix<N, M, T>::column_t& Matrix<N, M, T>::operator[](size_t i)
    {
        assert(i >= 0 && i < N);
        return m_data[i];
    }

    template <size_t N, size_t M, typename T>
    constexpr const typename Matrix<N, M, T>::column_t& Matrix<N, M, T>::operator[](size_t i) const
    {
        assert(i >= 0 && i < N);
        return m_data[i];
    }

    template <size_t N, size_t M, typename T>
    constexpr Matrix<N, M, T>& Matrix<N, M, T>::operator=(const Matrix<N, M, T> &other) 
    {
        m_data = other.m_data;
        return *this;
    }

    template <size_t N, size_t M, typename T>
    constexpr Matrix<N, M, T> operator+(const Matrix<N, M, T> &m1, const Matrix<N, M, T> &m2)
    {
        Matrix<N, M, T> ret = m1;
        ret += m2;
        return ret;
    }

    template <size_t N, size_t M, typename T>
    constexpr Matrix<N, M, T> operator-(const Matrix<N, M, T> &m1, const Matrix<N, M, T> &m2)
    {
        Matrix<N, M, T> ret = m1;
        ret -= m2;
        return ret;
    }

    template <size_t N, size_t M, size_t M2, typename T>
    constexpr Matrix<M2, M, T> operator*(const Matrix<N, M, T> &m1, const Matrix<M2, N, T> &m2) 
    {
        Matrix<M2, M, T> ret;
        Matrix<N, M, T> _m1 = m1.transpose();

        for (size_t i = 0; i < M; ++i)
        {
            for (size_t j = 0; j < M2; ++j)
            {
                auto v = _m1.m_data[i];
                auto& v2 = m2.m_data[j];
                T val = static_cast<T>(0.0);
                v *= v2;
                val = std::reduce(v.m_data.begin(), v.m_data.end(), static_cast<T>(0.0));
                ret[j][i] = val;
            }
        }

        return ret;
    }

    template <size_t N, size_t M, typename T>
    inline const Matrix<N, M, T> Matrix<N, M, T>::identity = Matrix<N, M, T>::_genIdentity();
}