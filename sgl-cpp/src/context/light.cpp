#include "light.h"
#include "math/utils.h"
#include <cmath>
#include <cstdlib>

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

DirectionalLight::DirectionalLight(const vec3& dir, const vec3& color)
    : Light(color),
      m_dir(dir)
{
    
}

vec3 DirectionalLight::getDirection(const vec3& from) const 
{
    return -m_dir;
}

AreaLight::AreaLight(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& color, const float c0, const float c1, const float c2)
    : Light(color),
      m_v1(v1),
      m_e1(v2-v1),
      m_e2(v3-v1),
      m_normal(math::normalize(math::crossProduct(m_e1, m_e2))),
      m_areaOverSamples( (0.5 * math::length(math::crossProduct(m_e1, m_e2))) / SAMPLE_NUMBER),
      m_c0(c0),
      m_c1(c1),
      m_c2(c2)
{
}

vec3 AreaLight::getDirection(const vec3& from) const 
{
    return getRandomPoint() - from;
}

vec3 AreaLight::getColor(const vec3& direction) const 
{
    float d = math::length(direction);
    float cosfi = math::dotProduct(m_normal, -math::normalize(direction));
    return m_color * (cosfi * m_areaOverSamples / (m_c0 + m_c1 * d + m_c2 * d * d));
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

} // namespace sgl
