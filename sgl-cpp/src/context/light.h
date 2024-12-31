#pragma once

#include "math/vector.h"
#include <array>
#include <random>

namespace sgl
{

class Light
{
public:
    Light(Light&&) = default;
    Light(const Light&) = default;
    virtual ~Light() = default;

    Light(const vec3& color);

    // Return direction towards light
    virtual bool isObstructed(const vec3& point, const vec3& hit) const = 0;
    virtual vec3 getDirection(const vec3& from) const = 0;
    virtual bool isAreaLight() const;
    virtual vec3 getColor(const vec3& direction) const;

protected:
    vec3 m_color;
};

class PointLight : public Light
{
public:
    PointLight(PointLight&&) = default;

    PointLight(const vec3& pos, const vec3& color);

    virtual vec3 getDirection(const vec3& from) const;
    virtual bool isObstructed(const vec3& point, const vec3& middle) const;

    // friend Context;

    vec3 m_pos;
private:
};

class DirectionalLight : public Light
{
public:
    DirectionalLight(DirectionalLight&&) = default;

    DirectionalLight(const vec3& dir, const vec3& color);

    virtual vec3 getDirection(const vec3& from) const;
    virtual bool isObstructed(const vec3& point, const vec3& middle) const;

private:
    vec3 m_dir;
};

class AreaLight : public Light
{
public:
    static const int SAMPLE_NUMBER = 16;

    AreaLight(AreaLight&&) = default;

    AreaLight(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& color, const vec3& attenuation);

    virtual vec3 getDirection(const vec3& from) const;
    virtual bool isObstructed(const vec3& point, const vec3& middle) const;
    virtual vec3 getColor(const vec3& direction) const;
    virtual bool isAreaLight() const;

private:
    static const std::random_device m_dev;
    // static const std::uniform_real_distribution<> m_distribution;
    static const std::mt19937 m_generator( std::random_device() );

    const vec3 m_v1;
    const vec3 m_e1;
    const vec3 m_e2;

    const vec3 m_normal;
    const float m_area;

    const vec3 m_attenuation;

    vec3 getRandomPoint() const;
};

}