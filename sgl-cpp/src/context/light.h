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
    const vec3& getColor() const;

private:
    vec3 m_color;
};

class PointLight : public Light
{
public:
    PointLight(PointLight&&) = default;

    PointLight(const vec3& pos, const vec3& color);

    virtual vec3 getDirection(const vec3& from) const;

private:
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

}