#include "context/context.h"

#include "math/transform.h"
#include "sgl.h"

#include <cmath>
#include <corecrt_math_defines.h>
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
          m_elementType(SGL_LAST_ELEMENT_TYPE),
          m_viewport(mat4::identity),
          m_PVM(mat4::identity)
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

    void Context::drawLine(vec2 p1f, vec2 p2f) 
    {
        vec2i p1(p1f);
        vec2i p2(p2f);

        // int c0, c1, p;
        // c0 = 2 * (p2.y - p1.y);
        // c1 = c0 - 2 * (p2.x - p1.x);
        // p = c0 - (p2.x - p1.x);

        // putPixel(p1, m_drawColor);
        // for (int i = p1.x + 1; i <= p2.x && i < m_width && p1.y < m_height; ++i)
        // {
        //     if (p < 0)
        //     {
        //         p += c0;
        //     }
        //     else 
        //     {
        //         p += c1;
        //         ++p1.y;
        //         // p1.y += ydiff;
        //     }
        //     putPixel(i, p1.y, m_drawColor);
        // }

        int dx = std::abs(p2.x - p1.x);
        int dy = std::abs(p2.y - p1.y);

        int slopeX = p1.x < p2.x ? 1 : -1;
        int slopeY = p1.y < p2.y ? 1 : -1;

        int err = dx - dy;

        while (true)
        {
            putPixel(p1, m_drawColor);
            if (p1.x == p2.x && p1.y == p2.y) break;

            int e2 = 2 * err;

            if (e2 > -dy)
            {
                err -= dy;
                p1.x += slopeX;
            }

            if (e2 < dx)
            {
                err += dx;
                p1.y += slopeY;
            }
        }
    }

    void Context::setMatrixMode(uint8_t mode)
    {
        assert(m_isInitialized);
        m_isModelActive = mode == SGL_MODELVIEW;
    }

    void Context::setViewport(int x, int y, int width, int height) 
    {
        m_viewport = viewport(x, y, width, height);
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
        // assert(m_isInitialized && x >= 0 && y >= 0 && x < m_width && y < m_height);
        assert(m_isInitialized);
        // if (x >= 0 && y >= 0 && x < m_width && y < m_height)
        if (x < 0 || y < 0 || x >= m_width || y >= m_height)
        {
            return;
        }
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
        // m_PVM = m_viewport * getProjection() * getModelView();
        updatePVM();
    }

    void Context::addVertex(const vec4& vertex, bool applyPVM) 
    {
        assert(m_isDrawing);
        if (applyPVM) m_vertexBuffer.push_back(m_PVM * vertex);
        else m_vertexBuffer.push_back(vertex);
    }

    void Context::addVertex(const vec4& vertex, const mat4& matrix) 
    {
        assert(m_isDrawing);
        m_vertexBuffer.push_back(matrix * vertex);
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

    void Context::drawCircle(vec2 center, float radius) 
    {
        // vec2i c( center);

        vec2i c(m_PVM * vec4(center, 0, 1));

        const mat4& mv = m_PVM;
        auto mv00 = mv[0][0];
        auto mv10 = mv[1][0];
        auto mv20 = mv[2][0];
        float scale = std::sqrt(mv00 * mv00 + mv10 * mv10 + mv20 * mv20);
        radius *= scale;
        
        int x, y, p, fourX, fourY;
        x = 0;
        y = radius;
        p = 3 - 2*radius;
        fourX = 0;
        fourY = 4*radius;
        while (x <= y)
        {
            putPixel(x+c.x, y+c.y, m_drawColor);
            putPixel(x+c.x, -y+c.y, m_drawColor);
            putPixel(-x+c.x, y+c.y, m_drawColor);
            putPixel(-x+c.x, -y+c.y, m_drawColor);
            putPixel(y+c.x, x+c.y, m_drawColor);
            putPixel(y+c.x, -x+c.y, m_drawColor);
            putPixel(-y+c.x, x+c.y, m_drawColor);
            putPixel(-y+c.x, -x+c.y, m_drawColor);
            if (p > 0)
            {
                p -= fourY + 4;
                fourY -= 4;
                --y;
            }
            p += fourX + 6;
            fourX += 4;
            ++x;
        }
    }

    void Context::drawEllipse(vec2 center, float a, float b)
    {
        int steps = 40;
        float dtheta = 2 * M_PI / steps;

        vec2i c(m_PVM * vec4(center, 0, 1));

        const mat4& mv = m_PVM;
        auto mv00 = mv[0][0];
        auto mv10 = mv[1][0];
        auto mv20 = mv[2][0];
        float scale = std::sqrt(mv00 * mv00 + mv10 * mv10 + mv20 * mv20);
        a *= scale;
        b *= scale;

        beginDrawing(SGL_LINE_LOOP);
        for (int i = 0; i < steps; ++i)
        {
            float theta = i * dtheta;
            int x = static_cast<int>(std::round(c.x + a * std::cos(theta)));
            int y = static_cast<int>(std::round(c.y + b * std::sin(theta)));
            addVertex(vec4(x, y, 0, 1), false);
        }
        drawBuffer();
    }

    void Context::drawArc(vec2 center, float radius, float fromRad, float toRad)
    {    
        int steps = 40;
        float dtheta = (toRad-fromRad) / steps;

        vec2i c(m_PVM * vec4(center, 0, 1));

        const mat4& mv = m_PVM;
        auto mv00 = mv[0][0];
        auto mv10 = mv[1][0];
        auto mv20 = mv[2][0];
        float scale = std::sqrt(mv00 * mv00 + mv10 * mv10 + mv20 * mv20);

        radius *= scale;

        beginDrawing(SGL_LINE_STRIP);
        for (int i = 0; i < steps; ++i)
        {
            float theta = fromRad + i * dtheta;
            int x = static_cast<int>(std::round(c.x + radius * std::cos(theta)));
            int y = static_cast<int>(std::round(c.y + radius * std::sin(theta)));
            addVertex(vec4(x, y, 0, 1), false);
        }
        drawBuffer();

    }

    void Context::drawPoint(float x, float y, float z) 
    {
        float halfSize = m_pointSize * 0.5;
        for (long yp = std::max(std::lround(y-halfSize), 0L); yp < std::min(std::lround(y+halfSize), static_cast<long>(m_height)); ++yp)
        {
            long xp = std::max(std::lround(x-halfSize), 0L);
            long xl = std::min(std::lround(x+halfSize), static_cast<long>(m_width));
            auto startIt = std::next(m_colorBuffer.begin(), yp*m_width+xp);
            auto endIt = std::next(m_colorBuffer.begin(), yp*m_width+xl);
            std::fill(startIt, endIt, m_drawColor);
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

    const mat4& Context::getModelView() const 
    {
        return m_modelStack.back();
    }

    const mat4& Context::getProjection() const 
    {
        return m_projectionStack.back();
    }

    void Context::updatePVM()
    {
        m_PVM = m_viewport * getProjection() * getModelView();
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
        return stack.back();
    }

    std::vector<mat4>& Context::getCurrentStack()
    {
        assert(m_isInitialized && m_modelStack.size() > 0 && m_projectionStack.size() > 0);
        return m_isModelActive ? m_modelStack : m_projectionStack;
    }

} // namespace sgl
