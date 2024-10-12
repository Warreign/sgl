#include <cassert>
#include <algorithm>
#include <iostream>

namespace sgl 
{

namespace 
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
    template <typename I, typename>
    inline constexpr Vector<N, T>::Vector(const I (&l)[N]) {
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
    template <typename I, typename >
    inline constexpr Vector<N, T>& Vector<N, T>::operator=(const Vector<N, I>& other)
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
    template <typename I, typename>
    inline constexpr Vector<N, T>::Vector(I x, I y)
        : VectorData<2, T>(static_cast<T>(x), static_cast<T>(y)) {}

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
        this->m_data.fill(static_cast<T>(scalar));
    }

    template <size_t N, typename T>
    template <size_t M, typename U, typename >
    inline constexpr Vector<N, T>::Vector(const Vector<M, U>& other)
    {
        for (int i = 0; i < N; ++i)
        {
            this->m_data[i] = static_cast<T>(other.m_data[i]);
        }
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

}

}