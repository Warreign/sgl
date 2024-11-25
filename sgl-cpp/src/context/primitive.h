#pragma once

#include "material.h"
#include "math/vector.h"
#include "context/ray.h"

#include <array>
#include <initializer_list>
#include <tuple>
#include <vector>

namespace sgl
{

class Primitive
{

public:
    Primitive(const Primitive&) = delete;
    Primitive(Primitive&&) = delete;

    Primitive(const Material& material) :
        m_material(material)
    {}

    virtual ~Primitive() = default;

    // Intersect ray with the primitive, return point of intersection
    virtual std::tuple<bool, vec3> intersect(const Ray& ray) = 0;

private:
    Material m_material;

};

/*
// Polygon
class Polygon : public Primitive
{

public:
    Polygon() = delete;

    Polygon(std::initializer_list<vec4> il);
    Polygon(const std::vector<vec4>& vertices);

    virtual vec3 intersect(const Ray& ray) override;

private:
    std::vector<vec4> m_vertices;

};
*/

// Triangle
class Triangle : public Primitive
{

public:
    Triangle() = delete;

    Triangle(const Material& material, const vec3& v1, const vec3& v2, const vec3& v3);

    virtual std::tuple<bool, vec3> intersect(const Ray& ray) override;

private:
    std::array<vec4, 3> m_vertices;

};

// Sphere
class Sphere : public Primitive
{

public:
    Sphere() = delete;

    Sphere(const Material& material, const vec3& center, float radius);

    virtual std::tuple<bool, vec3> intersect(const Ray& ray) override;

private:
    vec3 m_center;
    float m_radius;

};

}
