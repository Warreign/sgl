#pragma once

#include <array>
#include <functional>
#include <type_traits>
#include <algorithm>

namespace sgl
{

namespace 
{
    template <size_t N, typename T>
    struct VectorData;

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
    class Vector : public VectorData<N, T>
    {
    public:
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
        constexpr Vector(const Vector<N, T>& other) = default;

        template <size_t M, typename U, typename = std::enable_if_t<N <= M>>
        constexpr Vector(const Vector<M, U>& other);


        // Move constructor
        constexpr Vector(Vector<N, T>&& other) = default;


        // List constructor
        template <typename I, typename = std::enable_if_t<std::is_arithmetic_v<I>>>
        constexpr Vector(const I (&l)[N]);


    private:
        struct Empty {};
        using T1 = std::conditional_t<N >= 1, T, Empty>;
        using T2 = std::conditional_t<N >= 2, T, Empty>;
        using T3 = std::conditional_t<N >= 3, T, Empty>;
        using T4 = std::conditional_t<N >= 4, T, Empty>;
    
    public:

        // Access operators
        constexpr T& operator[](size_t i);
        constexpr const T& operator[](size_t i) const;

        // Arithmetic vector operators
        constexpr T& operator+=(const Vector<N, T>& other);
        constexpr T& operator-=(const Vector<N, T>& other);
        constexpr T& operator*=(const Vector<N, T>& other);
        constexpr T& operator/=(const Vector<N, T>& other);

        // In/decrement operators
        constexpr T& operator++();
        constexpr T& operator--();
        constexpr T operator++(int);
        constexpr T operator--(int);

        // Assignment operator
        template <typename I, typename = std::enable_if_t<std::is_arithmetic_v<I>>>
        constexpr Vector<N, T>& operator=(const Vector<N, I>& other);

    };

    template <size_t N, typename T>
    inline constexpr Vector<N, T>::Vector()
    {
        m_data.fill(static_cast<T>(0));
    }

    template <size_t N, typename T>
    template <typename I, typename >
    inline constexpr Vector<N, T>::Vector(const I (&l)[N])
    {
        for (int i = 0; i < N; ++i)
        {
            m_data[i] = static_cast<T>(l[i]);
        }
    }

    template <size_t N, typename T>
    inline constexpr T& Vector<N, T>::operator[](size_t i)
    {
        return m_data[i];
    }

    template <size_t N, typename T>
    inline constexpr const T& Vector<N, T>::operator[](size_t i) const 
    {
        return m_data[i];
    }

    template <size_t N, typename T>
    inline constexpr T& Vector<N, T>::operator+=(const Vector<N, T>& other)
    {
        for (int i = 0; i < N; ++i)
        {
            m_data[i] += other.m_data[i];
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr T& Vector<N, T>::operator-=(const Vector<N, T>& other)
    {        
        for (int i = 0; i < N; ++i)
        {
            m_data[i] -= other.m_data[i];
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr T& Vector<N, T>::operator*=(const Vector<N, T>& other)
    {
        for (int i = 0; i < N; ++i)
        {
            m_data[i] *= other.m_data[i];
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr T& Vector<N, T>::operator/=(const Vector<N, T>& other)
    {
        for (int i = 0; i < N; ++i)
        {
            m_data[i] /= other.m_data[i];
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr T& Vector<N, T>::operator++()
    {
        for (int i = 0; i < N; ++i)
        {
            ++m_data[i];
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr T& Vector<N, T>::operator--()
    {
        for (int i = 0; i < N; ++i)
        {
            --m_data[i];
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr T Vector<N, T>::operator++(int)
    {
        Vector<N, T> ret = *this;
        ++*this;
        return ret;
    }

    template <size_t N, typename T>
    inline constexpr T Vector<N, T>::operator--(int)
    {
        Vector<N, T> ret = *this;
        --*this;
        return ret;
    }

    template <size_t N, typename T>
    template <typename I, typename >
    inline constexpr Vector<N, T>& Vector<N, T>::operator=(const Vector<N, I>& other)
    {
        for (int i = 0; i < N; ++i)
        {
            m_data[i] = static_cast<T>(other.m_data[i]);
        }
        return *this;
    }

    template <size_t N, typename T>
    template <typename I, typename >
    inline constexpr Vector<N, T>::Vector(I x, I y)
        : VectorData<2, T>(static_cast<T>(x), static_cast<T>(y))
    {
    }

    template <size_t N, typename T>
    template <typename I, typename >
    inline constexpr Vector<N, T>::Vector(I x, I y, I z)
        : VectorData<3, T>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z))
    {
    }

    template <size_t N, typename T>
    template <typename I, typename >
    inline constexpr Vector<N, T>::Vector(I x, I y, I z, I w)
        : VectorData<4, T>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z), static_cast<T>(w))
    {
    }

    
    template <size_t N, typename T>
    template <typename I, typename >
    inline constexpr Vector<N, T>::Vector(I scalar)
    {
        m_data.fill(static_cast<T>(scalar));
    }

    template <size_t N, typename T>
    template <size_t M, typename U, typename >
    inline constexpr Vector<N, T>::Vector(const Vector<M, U>& other)
    {
        for (int i = 0; i < N; ++i)
        {
            m_data[i] = static_cast<T>(other.m_data[i]);
        }
    }

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
    inline constexpr Vector<N, T> operator+(const Vector<N, T>& v1, const Vector<N, T>& v2)
    {
        Vector<N, T> res(v1);
        for (int i = 0; i < N; ++i)
        {
            res.m_data[i] += v2.m_data[i];
        }
        return res;
    }

    template <size_t N, typename T, typename S, typename >
    inline constexpr Vector<N, T> operator+(const Vector<N, T>& v1, S scalar)
    {
        Vector<N, T> res(v1);
        for (int i = 0; i < N; ++i)
        {
            res.m_data[i] += static_cast<T>(scalar);
        }
        return res;
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T> operator-(const Vector<N, T>& v1, const Vector<N, T>& v2)
    {
        Vector<N, T> res(v1);
        for (int i = 0; i < N; ++i)
        {
            res.m_data[i] -= v2.m_data[i];
        }
        return res;
    }

    template <size_t N, typename T, typename S, typename >
    inline constexpr Vector<N, T> operator-(const Vector<N, T>& v1, S scalar)
    {
        Vector<N, T> res(v1);
        for (int i = 0; i < N; ++i)
        {
            res.m_data[i] -= static_cast<T>(scalar);
        }
        return res;
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T> operator*(const Vector<N, T>& v1, const Vector<N, T>& v2)
    {
        Vector<N, T> res(v1);
        for (int i = 0; i < N; ++i)
        {
            res.m_data[i] *= v2.m_data[i];
        }
        return res;
    }

    template <size_t N, typename T, typename S, typename >
    inline constexpr Vector<N, T> operator*(const Vector<N, T>& v1, S scalar)
    {
        Vector<N, T> res(v1);
        for (int i = 0; i < N; ++i)
        {
            res.m_data[i] *= static_cast<T>(scalar);
        }
        return res;
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T> operator/(const Vector<N, T>& v1, const Vector<N, T>& v2)
    {
        Vector<N, T> res(v1);
        for (int i = 0; i < N; ++i)
        {
            res.m_data[i] /= v2.m_data[i];
        }
        return res;
    }

    template <size_t N, typename T, typename S, typename >
    inline constexpr Vector<N, T> operator/(const Vector<N, T>& v1, S scalar)
    {
        Vector<N, T> res(v1);
        for (int i = 0; i < N; ++i)
        {
            res.m_data[i] /= static_cast<T>(scalar);
        }
        return res;
    }

    template <size_t N, typename T>
    inline constexpr bool operator==(const Vector<N, T>& v1, const Vector<N, T>& v2)
    {
        return std::mismatch(v1.m_data.begin(), v1.m_data.end(), v2.m_data.begin(), std::not_equal_to<>()).first == v1.m_data.begin();
    }

    template <size_t N, typename T>
    inline constexpr bool operator!=(const Vector<N, T>& v1, const Vector<N, T>& v2)
    {
        return std::mismatch(v1.m_data.begin(), v1.m_data.end(), v2.m_data.begin(), std::equal_to<>()).first == v1.m_data.begin();
    }
    

} // namespace

// Vector type definitions
typedef Vector<2, float> vec2;
typedef Vector<3, float> vec3;
typedef Vector<4, float> vec4;

typedef Vector<2, double> vec2d;
typedef Vector<3, double> vec3d;
typedef Vector<4, double> vec4d;

} // namespace sgl
