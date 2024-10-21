#include "context/context.h"

#include "sgl.h"

#include <cmath>
#include <iostream>
#include <cassert>

#include <algorithm>

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

    Context::Context(uint32_t width, uint32_t height, uint32_t id)
        : m_id(id),
          m_width(width),
          m_height(height),
          m_isModelActive(true),
          m_isDrawing(false),
          m_drawColor(0.0, 0.0, 0.0),
          m_clearColor(0.0, 0.0, 0.0),
          m_pointSize(4),
          m_colorBuffer(width * height, sgl::vec3(0.0, 0.0, 0.0)),
          m_depthBuffer(width * height, 0),
          m_elementType(SGL_LAST_ELEMENT_TYPE)
    {
        m_modelStack.push_back(mat4::identity);
        m_projectionStack.push_back(mat4::identity);
        enableFeatures(SGL_DEPTH_TEST);
        m_isInitialized = true;

        std::cout << "Initializing context with dimensions: " << width << "x" << height << std::endl;
    }

    void Context::clearBuffers(unsigned what)
    {
        if (what & SGL_COLOR_BUFFER_BIT)
        {
            std::fill(m_colorBuffer.begin(), m_colorBuffer.end(), m_clearColor);
        }
        if (what & SGL_DEPTH_BUFFER_BIT)
        {
            std::fill(m_depthBuffer.begin(), m_depthBuffer.end(), 0);
        }
    }

    float* Context::colorBufferData()
    {
        if (m_colorBuffer.size() == 0) return nullptr;
        // return &(m_colorBuffer[0][0]);
        return reinterpret_cast<float*>(m_colorBuffer.data());
    }

    void Context::setClearColor(const vec3& color)
    {
        m_clearColor = color;
    }

    void Context::setDrawColor(const vec3& color) 
    {
        m_drawColor = color;
    }

    void Context::setPointSize(float newSize) 
    {    
        m_pointSize = newSize;
    }

    void Context::drawLine(vec2 p1, vec2 p2) 
    {
        int dx = abs(p2.x - p1.x);
        int dy = abs(p2.y - p1.y);
        int sx = (p1.x < p2.x) ? 1 : -1;
        int sy = (p1.y < p2.y) ? 1 : -1;

        int err = dx - dy;
        while (true) {
            // Draw point
            drawPoint(p1.x, p1.y, 0);

            //reached the end of the line?
            if (p1.x == p2.x && p1.y == p2.y) {
                break;
            }
            // error
            int e2 = 2 * err;

            if (e2 > -dy) {
                err -= dy;
                p1.x += sx;
            }

            if (e2 < dx) {
                err += dx;
                p1.y += sy;
            }
        }
    }

    void Context::setMatrixMode(uint8_t mode)
    {
        assert(m_isInitialized);
        m_isModelActive = mode == SGL_MODELVIEW;
    }

    void Context::enableFeatures(uint32_t features)
    {
        m_features |= features;
    }

    void Context::disableFeatures(uint32_t features)
    {        
        m_features &= ~features;
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

    void Context::beginDrawing(uint32_t elementType) 
    {
        m_elementType = elementType;
        m_isDrawing = true;
        m_vertexBuffer.clear();
    }

    void Context::addVertex(const vec4& vertex) 
    {
        assert(m_isDrawing);
        m_vertexBuffer.push_back(vertex);
    }

    void Context::drawBuffer() 
    {
        assert(m_elementType != SGL_LAST_ELEMENT_TYPE && m_isDrawing);
        if ((m_elementType > SGL_POINTS && m_vertexBuffer.size() < 2))
        {
            m_elementType = SGL_LAST_ELEMENT_TYPE;
            m_vertexBuffer.clear();
            return;
        }

        switch (m_elementType)
        {
            case SGL_POINTS:
                for (const auto& vert : m_vertexBuffer)
                {
                    drawPoint(vert.x, vert.y, vert.z);
                }
                break;
            case SGL_LINES:
                for (int i = 1; i < m_vertexBuffer.size(); i += 2)
                {
                    vec2 p1 = m_vertexBuffer[i-1];
                    vec2 p2 = m_vertexBuffer[i];
                    drawLine(p1, p2);
                }
                break;
            case SGL_LINE_STRIP:
                for (int i = 1; i < m_vertexBuffer.size(); ++i)
                {
                    vec2 p1 = m_vertexBuffer[i-1];
                    vec2 p2 = m_vertexBuffer[i];
                    drawLine(p1, p2);
                }
                break;
            case SGL_LINE_LOOP:
                for (int i = 1; i < m_vertexBuffer.size(); ++i)
                {
                    vec2 p1 = m_vertexBuffer[i-1];
                    vec2 p2 = m_vertexBuffer[i];
                    drawLine(p1, p2);
                }
                drawLine(m_vertexBuffer.back(), m_vertexBuffer.front());
                break;
        }
        m_isDrawing = false;
        m_elementType = SGL_LAST_ELEMENT_TYPE;
        m_vertexBuffer.clear();
    }

    void Context::drawPoint(float x, float y, float z) 
    {
        float halfSize = m_pointSize * 0.5;
        for (long yp = std::max(std::lround(y-halfSize), 0L); yp < std::min(std::lround(y+halfSize), static_cast<long>(m_height)); ++yp)
        {
            for (long xp = std::max(std::lround(x-halfSize), 0L); xp < std::min(std::lround(x+halfSize), static_cast<long>(m_width)); ++xp)
            {
                putPixel(x, y, m_drawColor);
            }
        }
    }

    int Context::getId() const
    {
        return m_id;
    }

    int Context::getWidth() const 
    {
        return m_width;
    }

    int Context::getHeight() const 
    {
        return m_height;
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
