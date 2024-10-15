#include "context/context.h"
#include "context/context_manager.h"
#include "sgl.h"

#include <cassert>
#include <memory>

namespace sgl
{

    Context::Context()
        : m_id(-1),
          m_width(0),
          m_height(0),
          m_isInitialized(false)
    {
    }

    Context::~Context()
    {
        m_isInitialized = false;
    }

    Context::Context(int width, int height, int id)
        : m_id(id),
          m_width(width),
          m_height(height),
          m_isModelActive(true),
          m_isDrawing(false)
    {
        assert(width > 0 && height > 0);
        m_colorBuffer.resize(width * height);
        m_depthBuffer.resize(width * height);
        m_modelStack.push_back(mat4::identity);
        m_projectionStack.push_back(mat4::identity);
        m_isInitialized = true;

        for (int i = height/2; i < height; ++i)
        {
            for (int j = width/2; j < width; ++j)
            {
                putPixel(j, i, sgl::vec3(0.5, 0.2, 0.2));
            }
        }
    }

    float* Context::colorBufferData()
    {
        if (m_colorBuffer.size() == 0) return nullptr;
        return &(m_colorBuffer[0][0]);
    }

    void Context::setMatrixMode(uint8_t mode)
    {
        assert(m_isInitialized);
        m_isModelActive = mode == SGL_MODELVIEW;
    }

    void Context::putPixel(int x, int y, const vec3& color)
    {
        assert(m_isInitialized && x >= 0 && y >= 0 && x < m_width && y < m_height);
        m_colorBuffer[point2idx(x, y)] = color;
    }

    void Context::putPixel(const vec2i& screenPos, const vec3& color)
    {
        putPixel(screenPos.x, screenPos.y, color);
    }

    int Context::getId() const
    {
        return m_id;
    }

    int Context::point2idx(int x, int y) const
    {
        return y * m_width + x;
    }

    bool Context::isDrawing() const
    {
        return m_isDrawing;
    }

    bool Context::isInitialized() const
    {
        return m_isInitialized;
    }

    mat4& Context::getCurrentMat()
    {
        auto& stack = getCurrentStack();
        return stack.front();
    }

    std::vector<mat4>& Context::getCurrentStack()
    {
        assert(m_isInitialized && m_modelStack.size() > 0 && m_projectionStack.size() > 0);
        return m_isModelActive ? m_modelStack : m_projectionStack;
    }

} // namespace sgl
