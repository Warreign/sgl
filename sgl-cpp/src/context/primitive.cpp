#include "primitive.h"
#include "math/utils.h"
#include <algorithm>
#include <memory>
#include "math/utils.h"
#include "math/internal/vector_internal.h"

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
Triangle::Triangle(std::shared_ptr<Material> material, const vec3& v1, const vec3& v2, const vec3& v3)
    : Primitive(material), 
      m_vertices({v1, v2, v3}),
      m_normal(math::normalize(math::crossProduct(v2-v1, v3-v1)))
{
}

std::tuple<bool, vec3, float> Triangle::intersect(const Ray& ray) const
{
	const vec3 p1 = m_vertices[0];
	const vec3 p2 = m_vertices[1];
	const vec3 p3 = m_vertices[2];
	vec3 e1 = p2 - p1;
	vec3 e2 = p3 - p1;
	vec3 s1 = sgl::math::crossProduct(ray.dir, e2);
	float divisor = sgl::math::dotProduct(s1, e1);
	if (divisor == 0.)
		return { false, {} , 0};
	float invDivisor = 1.f / divisor;
	vec3 d = ray.origin - p1;
	float b1 = sgl::math::dotProduct(d, s1) * invDivisor;
	if (b1 < 0. || b1 > 1.)
		return { false, {} , 0};
	
	vec3 s2 = sgl::math::crossProduct(d, e1);
	float b2 = sgl::math::dotProduct(ray.dir, s2) * invDivisor;
	if (b2 < 0. || b1 + b2 > 1.)
		return { false, {} , 0};

	float t = sgl::math::dotProduct(e2, s2) * invDivisor;
	/*if (t < ray.mint || t > ray.maxt)
		return false;*/

	if (t > 0)
	{
		vec3 intersectPoint = ray.origin + ray.dir * t;
		return { true, intersectPoint, t};
	}
	else
	{
		return {false, vec3(), 0};
	}
}

vec3 Triangle::getNormal(const vec3& point) const
{
    return m_normal;    
}

void Triangle::applyTransform(const mat4& matrix) 
{
    for (vec3& vertex : m_vertices)
    {
        vertex = matrix * vec4(vertex, 1);
    }
}


// Sphere
Sphere::Sphere(std::shared_ptr<Material> material, const vec3& center, float radius)
    : Primitive(material),
      m_center(center),
      m_radius(radius)
{
}

std::tuple<bool, vec3, float> Sphere::intersect(const Ray& ray) const
{
	float t = 0;
	vec3 intersectPoint(0, 0, 0);
	const vec3 dst = ray.origin - m_center;
	const float b = sgl::math::dotProduct(dst, ray.dir);
	const float c = sgl::math::dotProduct(dst, dst) - m_radius*m_radius;
	const float d = b * b - c;

	if (d > 0) {
		t = -b - sqrtf(d);
		if (t < 0.0f)
			t = -b + sqrtf(d);

		if (t < 0.0f)
			return {false, vec3(), 0};

		intersectPoint = ray.origin + ray.dir * t;

		return {true, intersectPoint, t};
	}
	return { false, {} , false};
}

vec3 Sphere::getNormal(const vec3& point) const
{
    return math::normalize(point - m_center);
}

void Sphere::applyTransform(const mat4& matrix) 
{
    m_center = matrix * vec4(m_center, 1);    
}

const Material& Primitive::getMaterial() const 
{
    return *m_material;
}

} // namespace sgl
