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

    void clearColor();
    float* colorBufferData();


    void applyToCurrentMat(const mat4& matrix);
    void setCurrentMat(const mat4& matrix);
    void setMatrixMode(uint8_t mode);

    bool isDrawing() const;
    bool isInitialized() const;

    mat4& getCurrentMat();
    int getId() const;

private:
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
    vec4 m_clearColor;
    std::vector<vec4> m_colorBuffer;

    // Depth buffer
    std::vector<uint8_t> m_depthBuffer;

};

} // namespace sgl
