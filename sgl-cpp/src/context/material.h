#pragma once

#include "math/vector.h"
#include <cstdint>
#include <string>

namespace sgl 
{

struct Material
{
    Material() = default;

    Material(const vec3& color, float kd, float ks, float shine, float T, float ior);

    virtual vec3 getColor(const vec2& texCoord = vec2()) const;
    virtual bool isEmissive() const;
    virtual ~Material() = default;

    // vec3 color; // { r, g, b }
    float kd;
    float ks;
    float shine;
    float T;
    float ior;

private:
    vec3 m_color;
};

struct EmissiveMaterial : public Material
{
    EmissiveMaterial(const vec3& color, const float c0, const float c1, const float c2);

    virtual bool isEmissive() const;

    float c0, c1, c2;
};

struct TexturedMaterial : public Material
{
    TexturedMaterial(const std::string& texturePath, const float kd, const float ks, const float shine, const float T, const float ior);

    ~TexturedMaterial();

    virtual vec3 getColor(const vec2& texCoord = vec2()) const override;

private:
    std::string m_texturePath;
    unsigned char* m_textureData;

    int m_textureWidth;
    int m_textureHeight;
    int m_textureChannels;
};

}