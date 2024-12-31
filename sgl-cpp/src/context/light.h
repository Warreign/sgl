#pragma once

#include "math/vector.h"

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

    vec3 m_pos;
};

class DirectionalLight : public Light
{
public:
    DirectionalLight(DirectionalLight&&) = default;

    DirectionalLight(const vec3& dir, const vec3& color);

    virtual vec3 getDirection(const vec3& from) const;

private:
    vec3 m_dir;
};

class AreaLight : public Light
{
public:
    static const int SAMPLE_NUMBER = 16;

    AreaLight(AreaLight&&) = default;

    AreaLight(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& color, const float c0, const float c1, const float c2);

    virtual vec3 getDirection(const vec3& from) const;
    virtual vec3 getColor(const vec3& direction) const;
    virtual bool isAreaLight() const;

private:
    vec3 getRandomPoint() const;

    const vec3 m_v1;
    const vec3 m_e1;
    const vec3 m_e2;

    const vec3 m_normal;
    const float m_areaOverSamples;

    const float m_c0;
    const float m_c1;
    const float m_c2;
};

}