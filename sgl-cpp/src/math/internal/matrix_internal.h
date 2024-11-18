#pragma once

#include "vector_internal.h"


#include <array>
#include <type_traits>
#include <ostream>

#ifdef SGL_SIMD
#include <immintrin.h>
#include <pmmintrin.h>
#pragma GCC target("sse3")
#pragma GCC target("avx")
#endif

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

        template <typename... U, typename = std::enable_if_t<sizeof...(U) == (N * M)>>
        constexpr Matrix(const U& ... values);

        constexpr Matrix(const T* ptr);

        // Addition assignment
        constexpr Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& other);
        constexpr Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& other);
        constexpr Matrix<N, M, T>& operator*=(const Matrix<M, N, T>& other);

        // Access operators
        constexpr column_t& operator[](size_t i);
        constexpr const column_t& operator[](size_t i) const;
        
        // Assignment operators
        constexpr Matrix<N, M, T>& operator=(const Matrix<N, M, T>& other);

        Matrix<M, N, T> transpose() const;

        template <size_t _M = M, size_t _N = N, typename _T = T, typename = std::enable_if<_M == _N>>
        constexpr Matrix<_M, _N, _T> inverse() const;

        constexpr const T* data_ptr() const;

        friend std::ostream &operator<<(std::ostream &os, const Matrix<N, M, T> &rhs)
        {
            Matrix<M, N, T> rhsT = rhs.transpose();
            os << rhsT[0].toString('|', '|');
            for (size_t i = 1; i < M; ++i)
            {
                os << "\n" << rhsT[i].toString('|', '|');
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
    constexpr Vector<M, T> operator*(const Matrix<N, M, T>& m, const Vector<M, T>& v);


#ifdef SGL_SIMD
    Matrix<4, 4, float> operator+(const Matrix<4, 4, float>& m1, const Matrix<4, 4, float>& m2);
    Matrix<4, 4, float> operator-(const Matrix<4, 4, float>& m1, const Matrix<4, 4, float>& m2);
    Matrix<4, 4, float> operator*(const Matrix<4, 4, float>& m1, const Matrix<4, 4, float>& m2);

    Vector<4, float> operator*(const Matrix<4, 4, float>& m, const Vector<4, float>& v);
    template <size_t N, size_t M, typename T>
    
    template <size_t _M, size_t _N, typename _T, typename >
    constexpr Matrix<_M, _N, _T> Matrix<N, M, T>::inverse() const
    {
        return Matrix<_M, _N, _T>();
    }


#endif
}

#include "matrix_internal.inl"
