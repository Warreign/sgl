#include "context.h"

#include "math/transform.h"
#include "sgl.h"

#include <cmath>
#include <iostream>
#include <cassert>

#include <algorithm>

#pragma GCC diagnostic ignored "-Wsign-compare"

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
          m_isDrawing(false),
          m_pointSize(4),
          m_viewport(mat4::identity),
          m_isModelActive(true),
          m_clearColor(0.0, 0.0, 0.0),
          m_drawColor(0.0, 0.0, 0.0),
          m_colorBuffer(width * height, sgl::vec3(0.0, 0.0, 0.0)),
          m_depthBuffer(width * height, 0),
          m_areaMode(SGL_LINE),
          m_elementType(SGL_LAST_ELEMENT_TYPE),
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

        int dx = std::abs(p2.x - p1.x);
        int dy = std::abs(p2.y - p1.y);

        int slopeX = p1.x < p2.x ? 1 : -1;
        int slopeY = p1.y < p2.y ? 1 : -1;

        int err = dx - dy;

        putPixel(p1, m_drawColor);
        while (p1.x != p2.x || p1.y != p2.y)
        {
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
            putPixel(p1, m_drawColor);
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
        assert(m_isInitialized);
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

    void Context::putLine(int startX, int endX, int y, const vec3& color)
    {
        assert(startX <= endX && y >= 0 && y < m_height);
        startX = std::max(startX, 0);
        endX = std::min(endX, static_cast<int>(m_width));
        auto startIt = m_colorBuffer.begin() + point2idx(startX, y);
        auto endIt = m_colorBuffer.begin() + point2idx(endX, y);
        std::fill(startIt, endIt, color);
    }

    void Context::putLine(const vec2i& start, const vec2i end, const vec3& color)
    {
        assert(start.y == end.y);
        putLine(start.x, end.x, start.y, color);
    }

    void Context::beginDrawing(uint32_t elementType) 
    {
        m_elementType = elementType;
        m_isDrawing = true;
        m_vertexBuffer.clear();
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

        int vertexCount = static_cast<int>(m_vertexBuffer.size());

        switch (m_elementType)
        {
            case SGL_POINTS:
                for (const auto& vert : m_vertexBuffer)
                {
                    drawPoint(vert.x, vert.y, vert.z);
                }
                break;

            case SGL_LINES:
                for (int i = 1; i < vertexCount; i += 2)
                {
                    vec3 p1 = m_vertexBuffer[i-1];
                    vec3 p2 = m_vertexBuffer[i];
                    if (m_areaMode == SGL_POINT)
                    {
                        drawPoint(p1);
                        drawPoint(p2);
                    }
                    else 
                    {
                        drawLine(p1, p2);
                    }
                }
                break;

            case SGL_LINE_STRIP:
                if (m_areaMode == SGL_POINT)
                {
                    for (int i = 0; i < vertexCount; ++i)
                    {
                        vec3 p = m_vertexBuffer[i];
                        drawPoint(p);
                    }
                }
                else 
                {
                    for (int i = 0; i < vertexCount-1; ++i)
                    {
                        vec3 p1 = m_vertexBuffer[i];
                        vec3 p2 = m_vertexBuffer[(i+1) % vertexCount];
                        drawLine(p1, p2);
                    }
                }
                break;

            case SGL_LINE_LOOP:
                if (m_areaMode == SGL_POINT)
                {
                    for (int i = 0; i < vertexCount; ++i)
                    {
                        vec3 p = m_vertexBuffer[i];
                        drawPoint(p);
                    }
                }
                else 
                {
                    for (int i = 0; i < vertexCount; ++i)
                    {
                        vec3 p1 = m_vertexBuffer[i];
                        vec3 p2 = m_vertexBuffer[(i+1) % vertexCount];
                        drawLine(p1, p2);
                    }
                }
                break;

            case SGL_POLYGON:   // Draw polygon the same way as a line loop
                if (m_vertexBuffer.size() > 2)
                {
                    if (m_areaMode == SGL_POINT)
                    {
                        for (int i = 0; i < vertexCount; ++i)
                        {
                            vec3 p = m_vertexBuffer[i];
                            drawPoint(p);
                        }
                    }
                    else 
                    {
                        for (int i = 0; i < vertexCount; ++i)
                        {
                            vec3 p1 = m_vertexBuffer[i];
                            vec3 p2 = m_vertexBuffer[(i+1) % vertexCount];
                            drawLine(p1, p2);
                        }
                    }
                    if (m_areaMode == SGL_FILL) // Fill if the appropriate fill mode is selected
                    {
                        fill(m_vertexBuffer);
                    }
                }
                break;
            case SGL_TRIANGLES:
                for (int i = 2; i < vertexCount; ++i)
                {
                    vec4 p1 = m_vertexBuffer[i-2];
                    vec4 p2 = m_vertexBuffer[i-1];
                    vec4 p3 = m_vertexBuffer[i];
                    drawLine(p1, p2);
                    drawLine(p2, p3);
                    drawLine(p3, p1);
                    fill({p1, p2, p3});
                }
            default:
                assert(false); // Not implemented
                break;
        }
        m_isDrawing = false;
        m_elementType = SGL_LAST_ELEMENT_TYPE;
        m_vertexBuffer.clear();
    }

    void Context::setAreaMode(uint32_t areaMode)
    {
        m_areaMode = areaMode;
    }

    void Context::drawCircle(vec3 center, float radius, bool fill) 
    {
        vec3 transformedCenter(m_PVM * vec4(center, 1));

        if (m_areaMode == SGL_POINT)
        {
            drawPoint(transformedCenter);
            return;
        }

        vec3i c = transformedCenter;

        const mat4& mv = m_PVM;
        auto mv00 = mv[0][0];
        auto mv10 = mv[1][0];
        
        auto mv01 = mv[0][1];
        auto mv11 = mv[1][1];

        float scale = std::sqrt(std::abs(mv00 * mv11 - mv01 * mv10));

        radius *= scale;
        
        int x, y, p, fourX, fourY;
        x = 0;
        y = radius;
        p = 3 - 2*radius;
        fourX = 0;
        fourY = 4*radius;
        while (x <= y)
        {
            if (m_areaMode == SGL_FILL)
            {
                putLine(c.x - x, c.x + x, c.y + y, m_drawColor);
                putLine(c.x - x, c.x + x, c.y - y, m_drawColor);
                putLine(c.x - y, c.x + y, c.y + x, m_drawColor);
                putLine(c.x - y, c.x + y, c.y - x, m_drawColor);
            }
            else 
            {
                putPixel( x+c.x,  y+c.y, m_drawColor);
                putPixel( x+c.x, -y+c.y, m_drawColor);
                putPixel(-x+c.x,  y+c.y, m_drawColor);
                putPixel(-x+c.x, -y+c.y, m_drawColor);
                putPixel( y+c.x,  x+c.y, m_drawColor);
                putPixel( y+c.x, -x+c.y, m_drawColor);
                putPixel(-y+c.x,  x+c.y, m_drawColor);
                putPixel(-y+c.x, -x+c.y, m_drawColor);
            }
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
    void Context::drawCirclePolar(vec3 center, float radius)
    {
        int steps = 40;
        float dtheta = 2 * M_PI / steps;

        vec3i c(center);

        beginDrawing(SGL_LINE_LOOP);
        for (int i = 0; i < steps; ++i)
        {
            float theta = i * dtheta;
            float x = center.x + radius * std::cos(theta);
            float y = center.y + radius * std::sin(theta);
            addVertex(vec4(x, y, center.z, 1));
        }
        drawBuffer();
    }

    void Context::drawEllipse(vec3 center, float a, float b)
    {
        int steps = 40;
        float dtheta = 2 * M_PI / steps;

        vec2i c(center);

        beginDrawing(m_areaMode == SGL_FILL ? SGL_POLYGON : SGL_LINE_LOOP);
        for (int i = 0; i < steps; ++i)
        {
            float theta = i * dtheta;
            float x = center.x + a * std::cos(theta);
            float y = center.y + b * std::sin(theta);
            addVertex(vec4(x, y, 0, 1));
        }
        drawBuffer();
    }

    void Context::drawArc(vec3 center, float radius, float fromRad, float toRad)
    {    
        int steps = 40;

        steps = static_cast<int>( static_cast<float>(steps) * (toRad-fromRad) / 2 * M_PI);

        float dtheta = (toRad-fromRad) / steps;

        bool isFilled = m_areaMode == SGL_FILL;
        std::vector<vec4> fillVertices;

        beginDrawing(SGL_LINE_STRIP);
        for (int i = 0; i < steps; ++i)
        {
            float theta = fromRad + i * dtheta;
            float x = center.x + radius * std::cos(theta);
            float y = center.y + radius * std::sin(theta);
            addVertex(vec4(x, y, center.z, 1));
        }
        if (isFilled)
        {
            fillVertices = m_vertexBuffer;
        }
        drawBuffer();

        if (isFilled)
        {
            fillVertices.push_back(m_PVM * vec4(center, 1));
            fill(fillVertices);
        }
    }

    void Context::drawPoint(float x, float y, float z) 
    {
        float halfSize = m_pointSize * 0.5;
        for (int yPixel = std::max(static_cast<int>(y-halfSize), 0); yPixel < std::min(static_cast<uint32_t>(y+halfSize), m_height); ++yPixel)
        {
            uint32_t xPixelStart = std::max(static_cast<int>(x-halfSize), 0);
            uint32_t xPixelEnd = x + halfSize;
            putLine(xPixelStart, xPixelEnd, yPixel, m_drawColor);
        }
    }

    void Context::drawPoint(vec3 pt)
    {
        drawPoint(pt.x, pt.y, pt.z);
    }

    void Context::fill(const std::vector<vec4>& vertices)
    {
        struct Edge
        {
            int yMax;
            float x;
            float inverseSlope;
        };

        auto yComparator = [](const auto& v1, const auto& v2) { return v1.y < v2.y; };
        auto [min, max] = std::minmax_element(vertices.begin(), vertices.end(), yComparator);
        assert(min != vertices.end() && max != vertices.end());
        int minY = (*min).y;
        int maxY = (*max).y;

        std::vector<std::vector<Edge>> edgeTable(maxY - minY + 1);

        for (int i = 0; i < vertices.size(); ++i)
        {
            vec3i p1 = vertices[i];
            vec3i p2 = vertices[(i+1) % vertices.size()];

            if (p1.y == p2.y)
            {
                continue;
            }

            if (p1.y > p2.y)
            {
                std::swap(p1, p2);
            }

            Edge e;
            e.yMax = p2.y;
            e.x = p1.x;
            e.inverseSlope = static_cast<float>(p2.x-p1.x) / static_cast<float>(p2.y-p1.y);
            edgeTable[p1.y - minY].emplace_back(e);
        }

        std::vector<Edge> activeTable;

        for (int y = minY; y < maxY; ++y)
        {
            if (y - minY < edgeTable.size())
            {
                auto& table = edgeTable[y - minY];
                activeTable.insert(activeTable.end(), table.begin(), table.end());
            }

            activeTable.erase(std::remove_if(activeTable.begin(), activeTable.end(), [y](const Edge& edge) { return edge.yMax <= y; }), activeTable.end());

            std::sort(activeTable.begin(), activeTable.end(), [](const auto& e1, const auto& e2) { return e1.x < e2.x; });

            for (int i = 1; i < activeTable.size(); i += 2)
            {
                int startX = static_cast<int>(activeTable[i-1].x);
                int endX = static_cast<int>(activeTable[i].x);
                putLine(startX, endX, y, m_drawColor);
            }

            for (auto& edge : activeTable)
            {
                edge.x += edge.inverseSlope;
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
