#pragma once

#include <array>
#include <cstring>
#include <type_traits>
#include <ostream>
#include <string>

namespace sgl
{

    template <size_t N, typename T>
    struct VectorData
    {
        constexpr VectorData() = default;
        constexpr VectorData(const T (& list)[N])
            : m_data(list)
        {
        }

        std::array<T, N> m_data;
    };

    template <typename T>
    struct VectorData<1, T>
    {
        union {
            struct {
                union { T x, r; };
            };
            std::array<T, 1> m_data;
        };
    };

    template <typename T>
    struct VectorData<2, T>
    {
        union {
            struct {
                union { T x, r; };
                union { T y, g; };
            };
            std::array<T, 2> m_data;
        };

    };

    template <typename T>
    struct VectorData<3, T>
    {
        union {
            struct {
                union { T x, r; };
                union { T y, g; };
                union { T z, b; };
            };
            std::array<T, 3> m_data;
        };
    };

    template <typename T>
    struct VectorData<4, T>
    {
        union {
            struct {
                union { T x, r; };
                union { T y, g; };
                union { T z, b; };
                union { T w, a; };
            };
            std::array<T, 4> m_data;
        };
    };


    template <size_t N, typename T>
    struct Vector : public VectorData<N, T>
    {

        using data_type = T;
        constexpr static std::size_t size = N;

        // Default initializer constructor
        constexpr Vector();
        constexpr Vector(Vector<N, T>&& /*other*/) = default;

        // Scalar constructor
        template <typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
        constexpr explicit Vector(U scalar);

        // Copy constructors 
        constexpr Vector(const Vector<N, T>&) = default;

        template <size_t M, typename U, typename... Y, typename = std::enable_if_t<sizeof...(Y) == (M < N ? N - M : 0) && (std::is_arithmetic_v<Y> && ...)>>
        constexpr Vector(const Vector<M, U>& v, const Y& ... rest);

        template <size_t M, typename U, typename = std::enable_if_t<M < N>>
        constexpr Vector(const Vector<M, U>& v);
        
        // List constructors
        template <typename U>
        constexpr Vector(const U (&l)[N]);

        template <size_t M, typename U, typename Y, typename = std::enable_if_t<M < N>>
        constexpr Vector(const Vector<M, U>& v, const Y (& rest)[N-M]);

        template <typename... U, typename = std::enable_if_t<sizeof...(U) == N && (std::is_arithmetic_v<U> && ...)>>
        constexpr Vector(const U& ... values);

        // Access operators
        constexpr T& operator[](size_t i);
        constexpr const T& operator[](size_t i) const;

        // Arithmetic vector operators
        constexpr Vector<N, T>& operator+=(const Vector<N, T>& other);
        template <typename S>
        constexpr Vector<N, T>& operator+=(S scalar);

        constexpr Vector<N, T>& operator-=(const Vector<N, T>& other);
        template <typename S>
        constexpr Vector<N, T>& operator-=(S scalar);

        constexpr Vector<N, T>& operator*=(const Vector<N, T>& other);
        template <typename S>
        constexpr Vector<N, T>& operator*=(S scalar);

        constexpr Vector<N, T>& operator/=(const Vector<N, T>& other);
        template <typename S>
        constexpr Vector<N, T>& operator/=(S scalar);

        // In/decrement operators
        constexpr Vector<N, T>& operator++();
        constexpr Vector<N, T>& operator--();
        constexpr Vector<N, T> operator++(int);
        constexpr Vector<N, T> operator--(int);

        // Assignment operator
        constexpr Vector<N, T>& operator=(const Vector<N, T>& other);

        template <typename U>
        constexpr Vector<N, T>& operator=(const Vector<N, U>& other);

        constexpr Vector<N, T>& operator=(Vector<N, T>&& other);

        constexpr T* data_ptr();
        constexpr const T* data_ptr() const;

        std::string toString(char start = '(', char end = ')') const;

        friend std::ostream &operator<<(std::ostream &os, const Vector<N, T> &rhs)
        {
            return os << rhs.toString();
        }

    };

    // Arithmetic operators
    template <size_t N, typename T>
    constexpr Vector<N, T> operator+(const Vector<N, T>& v1, const Vector<N, T>& v2);
    template <size_t N, typename T, typename S>
    constexpr Vector<N, T> operator+(const Vector<N, T>& v1, S scalar);
    template <size_t N, typename T, typename S>
    constexpr Vector<N, T> operator+(S scalar, const Vector<N, T>& v);

    template <size_t N, typename T>
    constexpr Vector<N, T> operator-(const Vector<N, T>& v1, const Vector<N, T>& v2);
    template <size_t N, typename T, typename S>
    constexpr Vector<N, T> operator-(const Vector<N, T>& v1, S scalar);
    template <size_t N, typename T, typename S>
    constexpr Vector<N, T> operator-(S scalar, const Vector<N, T>& v);

    template <size_t N, typename T>
    constexpr Vector<N, T> operator*(const Vector<N, T>& v1, const Vector<N, T>& v2);
    template <size_t N, typename T, typename S>
    constexpr Vector<N, T> operator*(const Vector<N, T>& v1, S scalar);
    template <size_t N, typename T, typename S>
    constexpr Vector<N, T> operator*(S scalar, const Vector<N, T>& v);

    template <size_t N, typename T>
    constexpr Vector<N, T> operator/(const Vector<N, T>& v1, const Vector<N, T>& v2);
    template <size_t N, typename T, typename S>
    constexpr Vector<N, T> operator/(const Vector<N, T>& v1, S scalar);
    template <size_t N, typename T, typename S>
    constexpr Vector<N, T> operator/(S scalar, const Vector<N, T>& v);

    template <size_t N, typename T>
    constexpr Vector<N, T> operator-(const Vector<N, T>& v);

    // Comparison operators
    template <size_t N, typename T>
    constexpr bool operator==(const Vector<N, T>& v1, const Vector<N, T>& v2);

    template <size_t N, typename T>
    constexpr bool operator!=(const Vector<N, T>& v1, const Vector<N, T>& v2);

    template <size_t N, typename T>
    constexpr T* Vector<N, T>::data_ptr()
    {
        return this->m_data.data();
    }

    template <size_t N, typename T>
    constexpr const T* Vector<N, T>::data_ptr() const
    {
        return this->m_data.data();
    }

} // namespace

#include "vector_internal.inl"
