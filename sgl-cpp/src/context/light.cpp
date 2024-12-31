#include "light.h"
#include "math/utils.h"
#include <cmath>
#include <cstdlib>
#include <random>

namespace sgl
{

Light::Light(const vec3& color)
    : m_color(color)
{
    
}

bool Light::isAreaLight() const 
{
    return false;    
}

PointLight::PointLight(const vec3& pos, const vec3& color)
    : Light(color),
    m_pos(pos)
{
    
}

vec3 Light::getColor(const vec3& direction) const 
{
    return m_color;
}

vec3 PointLight::getDirection(const vec3& from) const 
{
    return m_pos - from;    
}

bool PointLight::isObstructed(const vec3& point, const vec3& hit) const 
{
    vec3 lightDir = m_pos - point;
    float lightDistance = sgl::math::length(lightDir);

    vec3 hitDir = hit - point;
    float hitDistance = sgl::math::length(hitDir);

    return (hitDistance < lightDistance) &&
        (math::dotProduct(lightDir, hitDir) > 0);
}

DirectionalLight::DirectionalLight(const vec3& dir, const vec3& color)
    : Light(color),
      m_dir(dir)
{
    
}

vec3 DirectionalLight::getDirection(const vec3& from) const 
{
    return -m_dir;
}

bool DirectionalLight::isObstructed(const vec3& point, const vec3& hit) const 
{
    return true;    
}

AreaLight::AreaLight(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& color, const vec3& attenuation)
    : Light(color),
      m_v1(v1),
      m_e1(v2-v1),
      m_e2(v3-v1),
      m_normal(math::normalize(math::crossProduct(m_e1, m_e2))),
      m_area(0.5 * math::length(math::crossProduct(m_e1, m_e2))),
      m_attenuation(attenuation)
{
}

vec3 AreaLight::getDirection(const vec3& from) const 
{
    return getRandomPoint() - from;
}

bool AreaLight::isObstructed(const vec3& point, const vec3& middle) const 
{
    return false;
}

vec3 AreaLight::getColor(const vec3& direction) const 
{
    // float A = 0.5 * math::length(math::crossProduct(m_e1, m_e2));
    float d = math::length(direction);
    // return ( (A * math::dotProduct(m_normal, -math::normalize(direction))) / (SAMPLE_NUMBER * (m_attenuation.x + d * m_attenuation.y + d * d * m_attenuation.z)) ) * Light::getColor(direction);
    float cosfi = math::dotProduct(m_normal, -direction);
    return m_color * cosfi * (m_area / SAMPLE_NUMBER) / (m_attenuation.x + m_attenuation.y * d + m_attenuation.z * d * d);
}

bool AreaLight::isAreaLight() const 
{
    return true;    
}

vec3 AreaLight::getRandomPoint() const 
{
    float r1 = static_cast<float>(std::rand()) / RAND_MAX;
    float r2 = static_cast<float>(std::rand()) / RAND_MAX;
    float sqrtr1 = std::sqrt(r1);
    float u = 1 - sqrtr1;
    float v = (1 - r2) * sqrtr1;
    return m_v1 + u * m_e1 + v * m_e2;
}

// const std::uniform_real_distribution<> AreaLight::m_distribution 

} // namespace sgl
