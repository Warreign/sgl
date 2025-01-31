#pragma once

#include "material.h"
#include "math/vector.h"
#include "context/ray.h"
#include "math/matrix.h"

#include <array>
#include <initializer_list>
#include <memory>
#include <tuple>
#include <vector>

namespace sgl
{

class Primitive
{

public:
    Primitive(const Primitive&) = delete;
    Primitive(Primitive&&) = delete;

    Primitive(std::shared_ptr<Material>material) :
        m_material(material)
    {}

    virtual ~Primitive() = default;

    // Intersect ray with the primitive, return point of intersection
    virtual std::tuple<bool, vec3, float> intersect(const Ray& ray) const = 0;
    virtual vec3 getNormal(const vec3& point) const = 0;
    virtual void applyTransform(const mat4& matrix) = 0;
    virtual vec2 getTextureCoords(const vec3& point) const = 0;

    const Material& getMaterial() const;

private:
    std::shared_ptr<Material> m_material;

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

    Triangle(std::shared_ptr<Material> material, const vec3& v1, const vec3& v2, const vec3& v3, const vec3& t1 = vec3(), const vec3& t2 = vec3(), const vec3& t3 = vec3());

    virtual std::tuple<bool, vec3, float> intersect(const Ray& ray) const override;
    virtual vec3 getNormal(const vec3& point) const override;
    virtual void applyTransform(const mat4& matrix) override;
    virtual vec2 getTextureCoords(const vec3& point) const override;

private:
    std::array<vec3, 3> m_vertices;
    std::array<vec3, 3> m_textureCoords;
    vec3 m_normal;

    vec3 m_v0v1;
    vec3 m_v0v2;
    vec3 m_v1v2;
    vec3 m_v2v0;
    float m_area;
};

// Sphere
class Sphere : public Primitive
{

public:
    Sphere() = delete;

    Sphere(std::shared_ptr<Material> material, const vec3& center, float radius);

    virtual std::tuple<bool, vec3, float> intersect(const Ray& ray) const override;
    virtual vec3 getNormal(const vec3& point) const override;
    virtual void applyTransform(const mat4& matrix) override;
    virtual vec2 getTextureCoords(const vec3& point) const override;

private:
    vec3 m_center;
    float m_radius;

};

}
