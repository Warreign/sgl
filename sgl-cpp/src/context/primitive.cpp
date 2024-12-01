#include "primitive.h"
#include <algorithm>
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
Triangle::Triangle(const Material& material, const vec3& v1, const vec3& v2, const vec3& v3)
    : Primitive(material), 
      m_vertices({v1, v2, v3})
{
}

std::tuple<bool, vec3> Triangle::intersect(const Ray& ray)
{
	const vec3 p1 = m_vertices[0];
	const vec3 p2 = m_vertices[1];
	const vec3 p3 = m_vertices[2];
	vec3 e1 = p2 - p1;
	vec3 e2 = p3 - p1;
	vec3 s1 = sgl::math::crossProduct(ray.dir, e2);
	float divisor = sgl::math::dotProduct(s1, e1);
	if (divisor == 0.)
		return { false, {} };
	float invDivisor = 1.f / divisor;
	vec3 d = ray.origin - p1;
	float b1 = sgl::math::dotProduct(d, s1) * invDivisor;
	if (b1 < 0. || b1 > 1.)
		return { false, {} };
	
	vec3 s2 = sgl::math::crossProduct(d, e1);
	float b2 = sgl::math::dotProduct(ray.dir, s2) * invDivisor;
	if (b2 < 0. || b1 + b2 > 1.)
		return { false, {} };

	float t = sgl::math::dotProduct(e2, s2) * invDivisor;
	/*if (t < ray.mint || t > ray.maxt)
		return false;*/

	vec3 intersectPoint = ray.origin + ray.dir * t;

	return { true, intersectPoint };
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

		intersectPoint = ray.origin + ray.dir * t;

		return {true, intersectPoint };
	}
	return { false, {} };
}

} // namespace sgl
