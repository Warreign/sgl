#pragma once

#include <array>
#include <type_traits>
#include <ostream>

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
        template <typename I, typename = std::enable_if_t<std::is_arithmetic_v<I>>>
        constexpr Vector<N, T>& operator=(const Vector<N, I>& other);

        template <typename I, typename = std::enable_if_t<std::is_arithmetic_v<I>>>
        constexpr Vector<N, T>& operator=(Vector<N, I> other);
        
        friend std::ostream &operator<<(std::ostream &os, const Vector<N, T> &rhs)
        {
            os << "(" << rhs[0];
            for (size_t i = 1; i < N; ++i)
            {
                os << ",\n " << rhs[i];
            }
            os << ")";
            return os;
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

} // namespace

} // namespace sgl

#include "vector_internal.inl"
