#include "light.h"

namespace sgl
{

PointLight::PointLight(const vec3& pos, const vec3& color)
    : m_pos(pos),
      m_color(color)
{
    
}

} // namespace sgl
