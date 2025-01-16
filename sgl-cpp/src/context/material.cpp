#include "material.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace sgl
{

TexturedMaterial::TexturedMaterial(const std::string& texturePath, const float kd, const float ks, const float shine, const float T, const float ior) :
    Material(vec3(), kd, ks, shine, T, ior), m_texturePath(texturePath)
{
    m_textureData = stbi_load(texturePath.c_str(), &m_textureWidth, &m_textureHeight, &m_textureChannels, 3);
    assert(m_textureData);
}

TexturedMaterial::~TexturedMaterial()
{
    stbi_image_free(m_textureData);
}

vec3 TexturedMaterial::getColor(const vec2& texCoord) const 
{
    if (texCoord.x < 0 || texCoord.x > 1 || texCoord.y < 0 || texCoord.y > 1)
    {
        return vec3(0, 0, 0);
    }
    int x = texCoord.x * m_textureWidth;
    int y = texCoord.y * m_textureHeight;
    int idx = (y * m_textureWidth + x) * m_textureChannels;
    return vec3(m_textureData[idx] / 255.f, m_textureData[idx+1] / 255.f, m_textureData[idx+2] / 255.f);
}

EmissiveMaterial::EmissiveMaterial(const vec3& color, const float c0, const float c1, const float c2) :
    Material(color, 1, 0, 0, 0, 1), c0(c0), c1(c1), c2(c2)
{
}

bool EmissiveMaterial::isEmissive() const 
{
    return true;    
}

Material::Material(const vec3& color, float kd, float ks, float shine, float T, float ior) :
    kd(kd), ks(ks), shine(shine), T(T), ior(ior), m_color(color)
{
}

vec3 Material::getColor(const vec2& texCoord) const 
{
    return m_color;
}

bool Material::isEmissive() const 
{
    return false;    
}
   
} // namespace sgl