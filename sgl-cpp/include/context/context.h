#pragma once

#include "utils/common.h"

#include "math/vector.h"
#include "math/matrix.h"

#include <vector>
#include <cstdint>

namespace sgl
{
    
class Context
{

public:
    Context() = delete;
    Context(Context& /* other */) = delete;

    explicit Context(int width, int height, int id);

    void clearColor();
    float* colorBufferData();

    void applyToCurrentMat(const mat4& matrix);
    void setCurrentMat(const mat4& matrix);

    inline bool isDrawing() const { return m_isDrawing; }

    inline int getId() const { return m_id; };

private:
    const int m_id;

    const int m_width;
    const int m_height;

    bool m_isDrawing;

    // Matrices
    mat4 m_matModelView;
    mat4 m_matProjection;
    mat4& m_matCurrent;

    // Color buffer
    vec4 m_clearColor;
    std::vector<vec4> m_colorBuffer;

    // Depth buffer
    std::vector<uint8_t> m_depthBuffer;

};

} // namespace sgl
