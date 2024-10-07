#pragma once

#include <array>
#include <type_traits>

namespace sgl
{

namespace 
{

    template <size_t N, typename T>
    class Vector
    {
    public:

        // Default initializer constructor
        constexpr Vector(void);
    

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

        // template <size_t N2, typename T2, typename = std::enable_if_t<N <= N2>>
        // constexpr Vector(const Vector<N2, T2>& other);


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
        union {
            struct {
                union { T1 x, r; };
                union { T2 y, g; };
                union { T3 z, b; };
                union { T4 w, a; };
            };
            std::array<T, N> m_data;
        };

        constexpr T& operator[](size_t i);
        constexpr const T& operator[](size_t i) const;

    };

    template <size_t N, typename T>
    inline constexpr Vector<N, T>::Vector(void)
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
    template <typename I, typename >
    inline constexpr Vector<N, T>::Vector(I x, I y)
        : x(static_cast<T>(x)),
          y(static_cast<T>(y))
    {
    }

    template <size_t N, typename T>
    template <typename I, typename >
    inline constexpr Vector<N, T>::Vector(I x, I y, I z)
        : x(static_cast<T>(x)),
          y(static_cast<T>(y)),
          z(static_cast<T>(z))
    {
    }

    template <size_t N, typename T>
    template <typename I, typename >
    inline constexpr Vector<N, T>::Vector(I x, I y, I z, I w)
        : x(static_cast<T>(x)),
          y(static_cast<T>(y)),
          z(static_cast<T>(z)),
          w(static_cast<T>(w))
    {
    }

    
    template <size_t N, typename T>
    template <typename I, typename >
    inline constexpr Vector<N, T>::Vector(I scalar)
    {
        m_data.fill(static_cast<T>(scalar));
    }

    // template <size_t N, typename T>
    // template <size_t N2, typename T2, typename >
    // inline constexpr Vector<N, T>::Vector(const Vector<N2, T2>& other)
    // {
    // }

} // namespace

typedef Vector<2, double> vec2;
typedef Vector<3, double> vec3;
typedef Vector<4, double> vec4;

typedef Vector<2, float> vec2f;
typedef Vector<3, float> vec3f;
typedef Vector<4, float> vec4f;

} // namespace sgl
