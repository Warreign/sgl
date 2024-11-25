#pragma once
#include "light.h"
#include "math/vector.h"
#include "math/matrix.h"
#include "primitive.h"

#include <bitset>
#include <functional>
#include <memory>
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

    void beginPrimitive(uint32_t elementType);
    void addVertex(const vec4& vertex, bool applyModelView = false);
    void addVertex(const vec4& vertex, const mat4& matrix);
    void endPrimitive();
    void drawBuffer();

    void beginScene();
    void endScene();
    bool isSpecifyingScene() const;
    void renderScene();
    void setCurrentMat(const Material& material);
    void addPointLight(PointLight&& pl);

    void addSphere(const vec3& center, float radius);

    void setAreaMode(uint32_t areaMode);
    
    void drawCircle(vec3 center, float radius, bool fill = false);
    void drawCirclePolar(vec3 center, float radius);
    void drawEllipse(vec3 center, float a, float b);
    void drawArc(vec3 center, float radius, float fromRad, float toRad);

    void updatePVM();

private:

// Pixel handling
    inline void putPixel(int x, int y, const vec3& color);
    inline void putPixelDepth(int x, int y, float z, const vec3& color);
    inline void putPixel(const vec3& pos, const vec3& color);
    inline void putPixelDepth(const vec3& pos, const vec3& color);

    inline void putLine(int startX, int endX, int y, const vec3& color);
    inline void putLineDepth(int startX, int endX, int y, float startZ, float endZ, const vec3& color);
    inline void putLine(const vec3& start, const vec3& end, const vec3& color);
    inline void putLineDepth(const vec3& start, const vec3& end, const vec3& color);

    inline int point2idx(int x, int y) const;

// Primitive handling
    void drawLine(vec3 p1, vec3 p2);
    void drawPoint(float x, float y, float z);
    void drawPoint(vec3 pt);

    void fill(const std::vector<vec4>& vertices);
    void fillDepth(const std::vector<vec4>& vertices);

// Transformation getters
    inline const mat4& getModelView() const;
    inline const mat4& getProjection() const;


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
    std::vector<float> m_depthBuffer;
    uint32_t m_areaMode;
    std::function<void(const std::vector<vec4>&)> m_fillFunc;

    // Vertex data
    std::vector<vec4> m_vertexBuffer;
    uint32_t m_elementType;
    mat4 m_PVM;

    // Scene data
    bool m_isSpecifyingScene;
    std::vector<std::unique_ptr<Primitive>> m_scenePrimitives;
    std::vector<Light> m_sceneLights;
    Material m_currentMat;

};

} // namespace sgl
