#pragma once

#include <array>
#include <type_traits>
#include <ostream>
#include <string>
#include <sstream>
#include <winscard.h>

namespace sgl
{

namespace 
{
    
    template <size_t N, typename T>
    struct VectorData;

    template <typename T>
    struct VectorData<1, T>
    {
        constexpr VectorData() = default;
        constexpr VectorData(T x)
            : x(x)
        {
        }

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
        constexpr VectorData() = default;
        constexpr VectorData(T x, T y)
            : x(x), y(y)
        {
        }

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
        constexpr VectorData() = default;
        constexpr VectorData(T x, T y, T z)
            : x(x), y(y), z(z)
        {
        }

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
        constexpr VectorData() = default;
        constexpr VectorData(T x, T y, T z, T w)
            : x(x), y(y), z(z), w(w)
        {
        }

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
        constexpr Vector(Vector<N, T>&& other);

        // Value arguments constructors
        template <typename I, typename = std::enable_if_t<N == 2 && std::is_arithmetic_v<I>>>
        constexpr Vector(I x, I y);

        template <typename I, typename = std::enable_if_t<N == 3 && std::is_arithmetic_v<I>>>
        constexpr Vector(I x, I y, I z);

        template <typename I, typename = std::enable_if_t<N == 4 && std::is_arithmetic_v<I>>>
        constexpr Vector(I x, I y, I z, I w);


        // Scalar constructor
        template <typename I, typename = std::enable_if_t<std::is_arithmetic_v<I>>>
        constexpr explicit Vector(I scalar);


        // Copy constructors 
        constexpr Vector(const Vector<N, T>&) = default;

        template <size_t M, typename U, typename = std::enable_if_t<N <= M>>
        constexpr Vector(const Vector<M, U>& other);


        // List constructor
        template <typename I, typename = std::enable_if_t<std::is_arithmetic_v<I>>>
        constexpr Vector(const I (&l)[N]);

        // Access operators
        constexpr T& operator[](size_t i);
        constexpr const T& operator[](size_t i) const;

        // Arithmetic vector operators
        constexpr Vector<N, T>& operator+=(const Vector<N, T>& other);
        constexpr Vector<N, T>& operator-=(const Vector<N, T>& other);
        constexpr Vector<N, T>& operator*=(const Vector<N, T>& other);
        constexpr Vector<N, T>& operator/=(const Vector<N, T>& other);

        // In/decrement operators
        constexpr Vector<N, T>& operator++();
        constexpr Vector<N, T>& operator--();
        constexpr Vector<N, T> operator++(int);
        constexpr Vector<N, T> operator--(int);

        // Assignment operator
        constexpr Vector<N, T>& operator=(const Vector<N, T>& other);

        template <typename I, typename = std::enable_if_t<std::is_arithmetic_v<I>>>
        constexpr Vector<N, T>& operator=(const Vector<N, I>& other);

        constexpr Vector<N, T>& operator=(Vector<N, T>&& other);

        std::string toString(char start = '(', char end = ')') const;

        friend std::ostream &operator<<(std::ostream &os, const Vector<N, T> &rhs)
        {
            return os << rhs.toString();
        }

    };

    // Arithmetic operators
    template <size_t N, typename T>
    constexpr Vector<N, T> operator+(const Vector<N, T>& v1, const Vector<N, T>& v2);
    template <size_t N, typename T, typename S, typename = std::enable_if_t<std::is_arithmetic_v<S>>>
    constexpr Vector<N, T> operator+(const Vector<N, T>& v1, S scalar);

    template <size_t N, typename T>
    constexpr Vector<N, T> operator-(const Vector<N, T>& v1, const Vector<N, T>& v2);
    template <size_t N, typename T, typename S, typename = std::enable_if_t<std::is_arithmetic_v<S>>>
    constexpr Vector<N, T> operator-(const Vector<N, T>& v1, S scalar);

    template <size_t N, typename T>
    constexpr Vector<N, T> operator*(const Vector<N, T>& v1, const Vector<N, T>& v2);
    template <size_t N, typename T, typename S, typename = std::enable_if_t<std::is_arithmetic_v<S>>>
    constexpr Vector<N, T> operator*(const Vector<N, T>& v1, S scalar);

    template <size_t N, typename T>
    constexpr Vector<N, T> operator/(const Vector<N, T>& v1, const Vector<N, T>& v2);
    template <size_t N, typename T, typename S, typename = std::enable_if_t<std::is_arithmetic_v<S>>>
    constexpr Vector<N, T> operator/(const Vector<N, T>& v1, S scalar);

    // Comparison operators
    template <size_t N, typename T>
    constexpr bool operator==(const Vector<N, T>& v1, const Vector<N, T>& v2);

    template <size_t N, typename T>
    constexpr bool operator!=(const Vector<N, T>& v1, const Vector<N, T>& v2);

    template <size_t N, typename T>
    inline std::string Vector<N, T>::toString(char start, char end) const
    {
        std::stringstream ss;
        ss << start << this->m_data[0];
        for (int i = 1; i < N; ++i)
        {
            ss << ", " << this->m_data[i];
        }
        ss << end;
        return ss.str();
    }

} // namespace

} // namespace sgl

#include "vector_internal.inl"
