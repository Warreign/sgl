#include "context/context.h"

#include <cassert>

namespace sgl
{

    Context::Context(int width, int height, int id)
        : m_id(id),
          m_width(width),
          m_height(height),
          m_matModelView(mat4::identity),
          m_matProjection(mat4::identity),
          m_matCurrent(m_matModelView)
    {
        assert(width > 0 && height > 0);
        m_colorBuffer.resize(width * height);
        m_depthBuffer.resize(width * height);
    }

    float* Context::colorBufferData()
    {
        if (m_colorBuffer.size() == 0) return nullptr;
        return &(m_colorBuffer[0][0]);
    }

    int Context::getId() const
    {
        return m_id;
    }

    bool Context::isDrawing() const
    {
        return m_isDrawing;
    }

} // namespace sgl
