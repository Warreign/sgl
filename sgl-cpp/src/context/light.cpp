#include "light.h"
#include "math/utils.h"

namespace sgl
{

Light::Light(const vec3& color)
    : m_color(color)
{
    
}

PointLight::PointLight(const vec3& pos, const vec3& color)
    : Light(color),
    m_pos(pos)
{
    
}

const vec3& Light::getColor() const 
{
    return m_color;
}

const vec3& PointLight::getDirection(const vec3& from) const 
{
    return math::normalize(m_pos - from);    
}

DirectionalLight::DirectionalLight(const vec3& dir, const vec3& color)
    : Light(color),
      m_dir(math::normalize(dir))
{
    
}

const vec3& DirectionalLight::getDirection(const vec3& from) const 
{
    return m_dir;
}

} // namespace sgl
