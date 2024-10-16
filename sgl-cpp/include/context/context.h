#pragma once

#include "utils/common.h"

#include "math/vector.h"
#include "math/matrix.h"

#include <bitset>
#include <vector>
#include <cstdint>

namespace sgl
{
    
class Context
{

public:
    Context();
    Context(const Context& /* other */) = delete;
    Context(Context&& other) = default;
    ~Context();

    Context& operator=(Context&& other) = default;

    explicit Context(int width, int height, int id);

    
    void setClearColor(const vec3 color);
    void clearBuffers(unsigned what);
    float* colorBufferData();

    mat4& getCurrentMat();
    std::vector<mat4>& getCurrentStack();
    void setMatrixMode(uint8_t mode);

    void putPixel(int x, int y, const vec3& color);
    void putPixel(const vec2i& screenPos, const vec3& color);

    bool isDrawing() const;
    bool isInitialized() const;

    int getId() const;

private:

    int point2idx(int x, int y) const;

    int m_id;

    int m_width;
    int m_height;

    bool m_isDrawing;
    bool m_isInitialized;

    // Matrices
    std::vector<mat4> m_modelStack;
    std::vector<mat4> m_projectionStack;
    bool m_isModelActive;

    // Color buffer
    vec3 m_clearColor;
    std::vector<vec3> m_colorBuffer;

    // Depth buffer
    std::vector<uint8_t> m_depthBuffer;

};

} // namespace sgl
