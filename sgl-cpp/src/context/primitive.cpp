#include "primitive.h"
#include <algorithm>

namespace sgl
{

/*
// Polygon
Polygon::Polygon(std::initializer_list<vec4> il)
    : m_vertices(il)
{
}

Polygon::Polygon(const std::vector<vec4>& vertices)
    : m_vertices(vertices)
{
}

vec3 Polygon::intersect(const Ray& ray)
{
    return {};
}
*/

// Triangle
Triangle::Triangle(const Material& material, const vec3& v1, const vec3& v2, const vec3& v3)
    : Primitive(material), 
      m_vertices({v1, v2, v3})
{
}

std::tuple<bool, vec3> Triangle::intersect(const Ray& ray)
{
    return {false, {}};
}


// Sphere
Sphere::Sphere(const Material& material, const vec3& center, float radius)
    : Primitive(material),
      m_center(center),
      m_radius(radius)
{
}

std::tuple<bool, vec3> Sphere::intersect(const Ray& ray)
{
    return {false, {}};
}

} // namespace sgl
