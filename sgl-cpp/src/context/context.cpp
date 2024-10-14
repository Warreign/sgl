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

    // Context::Context(Context&& other)
    //     : m_id(other.m_id),
    //       m_width(other.m_width),
    //       m_height(other.m_height),
    //       m_isDrawing(other.m_isDrawing),
    //       m_isInitialized(other.m_isInitialized),
    //       m_isModelActive(other.m_isModelActive),
    //       m_clearColor(other.m_clearColor),
    //       m_colorBuffer(std::move(other.m_colorBuffer)),
    //       m_depthBuffer(std::move(other.m_depthBuffer)),
    //       m_modelStack(std::move(other.m_modelStack)),
    //       m_projectionStack(std::move(other.m_projectionStack))
    // {
    // }

    Context::~Context()
    {
        m_isInitialized = false;
    }

    // Context& Context::operator=(Context&& other)
    // {
    //     m_id = other.m_id;
    //     m_width = other.m_width;
    //     m_height = other.m_height;
    //     m_isDrawing = other.m_isDrawing;
    //     m_isInitialized = other.m_isInitialized;
    //     m_isModelActive = other.m_isModelActive;
    //     m_clearColor = other.m_clearColor;
    //     m_colorBuffer = std::move(other.m_colorBuffer);
    //     m_depthBuffer = std::move(other.m_depthBuffer);
    //     m_modelStack = std::move(other.m_modelStack);
    //     m_projectionStack = std::move(other.m_projectionStack);
    //     return *this;
    // }

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
    }

    float* Context::colorBufferData()
    {
        if (m_colorBuffer.size() == 0) return nullptr;
        return &(m_colorBuffer[0][0]);
    }

    void Context::setCurrentMat(const mat4& matrix)
    {
        assert(m_isInitialized);
        mat4& current = getCurrentMat();
        if (m_isDrawing)
        {
            ContextManager::getInstance().setError(SGL_INVALID_OPERATION);
            return;
        }
        current = matrix;
    }

    void Context::setMatrixMode(uint8_t mode)
    {
        assert(m_isInitialized);
        m_isModelActive = mode == SGL_MODELVIEW;
    }

    void Context::applyToCurrentMat(const mat4& matrix)
    {
        mat4& current = getCurrentMat();
        current *= matrix;
    }

    int Context::getId() const
    {
        return m_id;
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
        assert(m_isInitialized && m_modelStack.size() > 0 && m_projectionStack.size() > 0);
        return m_isModelActive ? m_modelStack.back() : m_projectionStack.back();
    }

} // namespace sgl
