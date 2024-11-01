#include "vector_internal.h"

#include <cassert>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace sgl 
{

// Class definitions
    template <size_t N, typename T>
    inline constexpr Vector<N, T>::Vector()
    {
        this->m_data.fill(static_cast<T>(0));
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T>::Vector(Vector<N, T> &&other) 
    {
        this->m_data = std::move(other.m_data);
    }

    template <size_t N, typename T>
    template <typename U>
    inline constexpr Vector<N, T>::Vector(const U (&l)[N]) 
    {
        for (int i = 0; i < N; ++i) {
            this->m_data[i] = static_cast<T>(l[i]);
        }
    }

    template <size_t N, typename T>
    inline constexpr T& Vector<N, T>::operator[](size_t i)
    {
        assert(i >= 0 && i < N);
        return this->m_data[i];
    }

    template <size_t N, typename T>
    inline constexpr const T& Vector<N, T>::operator[](size_t i) const 
    {
        assert(i >= 0 && i < N);
        return this->m_data[i];
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T>& Vector<N, T>::operator+=(const Vector<N, T>& other)
    {
        for (int i = 0; i < N; ++i)
        {
            this->m_data[i] += other.m_data[i];
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T>& Vector<N, T>::operator-=(const Vector<N, T>& other)
    {        
        for (int i = 0; i < N; ++i)
        {
            this->m_data[i] -= other.m_data[i];
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T>& Vector<N, T>::operator*=(const Vector<N, T>& other)
    {
        for (int i = 0; i < N; ++i)
        {
            this->m_data[i] *= other.m_data[i];
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T>& Vector<N, T>::operator/=(const Vector<N, T>& other)
    {
        for (int i = 0; i < N; ++i)
        {
            this->m_data[i] /= other.m_data[i];
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T>& Vector<N, T>::operator++()
    {
        for (int i = 0; i < N; ++i)
        {
            ++this->m_data[i];
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T>& Vector<N, T>::operator--()
    {
        for (int i = 0; i < N; ++i)
        {
            --this->m_data[i];
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T> Vector<N, T>::operator++(int)
    {
        Vector<N, T> ret = *this;
        ++*this;
        return ret;
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T> Vector<N, T>::operator--(int)
    {
        Vector<N, T> ret = *this;
        --*this;
        return ret;
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T>& Vector<N, T>::operator=(const Vector<N, T>& other)
    {
        this->m_data = other.m_data;
        return *this;
    }

    template <size_t N, typename T>
    template <typename U>
    inline constexpr Vector<N, T>& Vector<N, T>::operator=(const Vector<N, U>& other)
    {
        for (int i = 0; i < N; ++i)
        {
            this->m_data[i] = static_cast<T>(other.m_data[i]);
        }
        return *this;
    }

    template <size_t N, typename T>
    inline constexpr Vector<N, T>& Vector<N, T>::operator=(Vector<N, T>&& other) 
    {
        this->m_data = std::move(other.m_data);
        return *this;
    }
    
    template <size_t N, typename T>
    template <typename U, typename >
    inline constexpr Vector<N, T>::Vector(U scalar)
    {
        this->m_data.fill(static_cast<T>(scalar));
    }

// Non-class definitions

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

    template <size_t N, typename T, typename S>
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

    template <size_t N, typename T, typename S>
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

    template <size_t N, typename T, typename S>
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

    template <size_t N, typename T, typename S>
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

    template <size_t N, typename T>
    inline std::string Vector<N, T>::toString(char start, char end) const
    {
        std::stringstream ss;
        ss << start << std::setw(3) << this->m_data[0] << std::setw(0);
        for (int i = 1; i < N; ++i)
        {
            ss << ", " << std::setw(3) << this->m_data[i] << std::setw(0);
        }
        ss << end;
        return ss.str();
    }

    template <size_t N, typename T>
    template <size_t M, typename U, typename Y, typename >
    inline constexpr Vector<N, T>::Vector(const Vector<M, U>& v, const Y (& rest)[N-M])
        : Vector(v)
    {
        for (int i = M; i < N; ++i)
        {
            this->m_data[i] = static_cast<T>(rest[i-M]);
        }
    }

    template <size_t N, typename T>
    template <size_t M, typename U, typename... Y, typename >
    constexpr Vector<N, T>::Vector(const Vector<M, U>& v, const Y& ... rest)
    {
        for (size_t i = 0; i < std::min<size_t>(N, M); ++i)
        {
            this->m_data[i] = static_cast<T>(v[i]);
        }
        
        size_t i = M;
        ([&]()
        {
            this->m_data[i++] = static_cast<T>(rest);
        }(), ...);
    }

    template <size_t N, typename T>
    template <typename... U, typename >
    constexpr Vector<N, T>::Vector(const U& ... values)
    {
        int i = 0;
        ([&]{ this->m_data[i++] = static_cast<T>(values); } (), ...);
    }

}