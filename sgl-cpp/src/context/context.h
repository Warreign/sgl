#pragma once
#include "math/vector.h"
#include "math/matrix.h"

#include <bitset>
#include <functional>
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

    explicit Context(uint32_t width, uint32_t height, uint32_t id);

    void clearBuffers(unsigned what);
    float* colorBufferData();
    void setClearColor(const vec3& color);
    void setDrawColor(const vec3& color); 
    void setPointSize(float newSize);

    mat4& getCurrentMat();
    std::vector<mat4>& getCurrentStack();
    void setMatrixMode(uint8_t mode);
    void setViewport(int x, int y, int width, int height);

    void enableFeatures(uint32_t features);
    void disableFeatures(uint32_t features);

    bool isDrawing() const;
    bool isInitialized() const;

    int getId() const;
    int getWidth() const;
    int getHeight() const;

    void beginDrawing(uint32_t elementType);
    void addVertex(const vec4& vertex, bool applyPVM = true);
    void addVertex(const vec4& vertex, const mat4& matrix);
    void drawBuffer();

    void setAreaMode(uint32_t areaMode);
    
    void drawCircle(vec3 center, float radius, bool fill = false);
    void drawCirclePolar(vec3 center, float radius);
    void drawEllipse(vec3 center, float a, float b);
    void drawArc(vec3 center, float radius, float fromRad, float toRad);

    void updatePVM();

private:

    inline void putPixel(int x, int y, const vec3& color);
    inline void putPixelDepth(int x, int y, int z, const vec3& color);
    inline void putPixel(const vec3i& screenPos, const vec3& color);
    inline void putPixelDepth(const vec3i& pos, const vec3& color);

    inline void putLine(int startX, int endX, int y, const vec3& color);
    inline void putLineDepth(int startX, int endX, int y, float startZ, float endZ, const vec3& color);
    inline void putLine(const vec3i& start, const vec3i& end, const vec3& color);
    inline void putLineDepth(const vec3i& start, const vec3i& end, const vec3& color);

    void drawLine(vec3 p1, vec3 p2);
    void drawPoint(float x, float y, float z);
    void drawPoint(vec3 pt);

    void fill(const std::vector<vec4>& vertices);
    void fillDepth(const std::vector<vec4>& vertices);

    int point2idx(int x, int y) const;

    const mat4& getModelView() const;
    const mat4& getProjection() const;


    int m_id;

    uint32_t m_width;
    uint32_t m_height;

    bool m_isDrawing;
    bool m_isInitialized;

    float m_pointSize;

    std::bitset<sizeof(uint32_t)*8> m_features;

    // Matrices
    std::vector<mat4> m_modelStack;
    std::vector<mat4> m_projectionStack;
    mat4 m_viewport;
    bool m_isModelActive;

    // Color buffer
    vec3 m_clearColor;
    vec3 m_drawColor;
    std::vector<vec3> m_colorBuffer;

    // Depth buffer
    std::vector<uint32_t> m_depthBuffer;
    uint32_t m_areaMode;
    std::function<void(const std::vector<vec4>&)> m_fillFunc;

    // Vertex data
    std::vector<vec4> m_vertexBuffer;
    uint32_t m_elementType;
    mat4 m_PVM;

};

} // namespace sgl
