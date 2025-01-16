#include "context.h"

#include "light.h"
#include "math/transform.h"
#include "math/utils.h"
#include "primitive.h"
#include "ray.h"
#include "sgl.h"

#include <cmath>
#include <functional>
#include <iostream>
#include <cassert>

#include <algorithm>
#include <limits>
#include <memory>

#pragma GCC diagnostic ignored "-Wsign-compare"

namespace sgl
{
    bool DOF = true;
    float apertureSize = 256;
    float focalLength = 900;//1005.5f;
    int numberOfRays = 16;

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
          m_pointSize(1),
          m_viewport(mat4::identity),
          m_isModelActive(true),
          m_clearColor(0.0, 0.0, 0.0),
          m_drawColor(0.0, 0.0, 0.0),
          m_colorBuffer(width * height, sgl::vec3(0.0, 0.0, 0.0)),
          m_depthBuffer(width * height, std::numeric_limits<float>::max()),
          m_areaMode(SGL_LINE),
          m_fillFunc(std::bind(&Context::fill, this, std::placeholders::_1)),
          m_elementType(SGL_LAST_ELEMENT_TYPE),
          m_PVM(mat4::identity),
          m_isSpecifyingScene(false),
          m_scenePrimitives(0)
    {
        m_modelStack.push_back(mat4::identity);
        m_projectionStack.push_back(mat4::identity);
        enableFeatures(SGL_DEPTH_TEST);
        m_isInitialized = true;
#ifdef DEBUG_MSG
        std::cout << "Initializing context with dimensions: " << width << "x" << height << std::endl;
#endif
    }

    void Context::clearBuffers(unsigned what)
    {
        if (what & SGL_COLOR_BUFFER_BIT)
        {
            std::fill(m_colorBuffer.begin(), m_colorBuffer.end(), m_clearColor);
        }
        if (what & SGL_DEPTH_BUFFER_BIT)
        {
            std::fill(m_depthBuffer.begin(), m_depthBuffer.end(), std::numeric_limits<float>::max());
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

    void Context::drawLine(vec3 p1f, vec3 p2f) 
    {
        vec3i p1(p1f);
        vec3i p2(p2f);

        int dx = std::abs(p2.x - p1.x);
        int dy = std::abs(p2.y - p1.y);

        int slopeX = p1.x < p2.x ? 1 : -1;
        int slopeY = p1.y < p2.y ? 1 : -1;

        int err = dx - dy;

        std::function putPixelFunc = [&](const vec3i& p, const vec3& color) { putPixel(p, color); };
        if (m_features.test(SGL_DEPTH_TEST))
        {
            putPixelFunc = [&](const vec3i& p, const vec3& c) { putPixelDepth(p, c); };
        }

        putPixelFunc(p1, m_drawColor);
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

            putPixelFunc(p1, m_drawColor);
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

        if (features & SGL_DEPTH_TEST)
        {
            using namespace std::placeholders;
            m_fillFunc = std::bind(&Context::fillDepth, this, _1);
        }
    }

    void Context::disableFeatures(uint32_t features)
    {        
        m_features &= ~features;

        if (features & SGL_DEPTH_TEST)
        {
            using namespace std::placeholders;
            m_fillFunc = std::bind(&Context::fill, this, _1);
        }
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

    void Context::putPixelDepth(int x, int y, float z, const vec3& color)
    {
        assert(m_isInitialized);
        if (x < 0 || y < 0 || x >= m_width || y >= m_height)
        {
            return;     
        }
        int idx = point2idx(x, y);
        if (z < m_depthBuffer[idx])
        {
            m_depthBuffer[idx] = z;
            m_colorBuffer[idx] = color;
        }
    }

    void Context::putPixel(const vec3& pos, const vec3& color)
    {
        putPixel(pos.x, pos.y, color);
    }

    void Context::putPixelDepth(const vec3& pos, const vec3& color)
    {
        putPixelDepth(pos.x, pos.y, pos.z, color);
    }

    void Context::putPixelRow(int startX, int endX, int y, const vec3& color)
    {
        assert(startX <= endX && y >= 0 && y < m_height);
        startX = std::max(startX, 0);
        endX = std::min(endX, static_cast<int>(m_width));
        auto startIt = m_colorBuffer.begin() + point2idx(startX, y);
        auto endIt = m_colorBuffer.begin() + point2idx(endX, y);
        std::fill(startIt, endIt, color);
    }

    void Context::putPixelRowDepth(int startX, int endX, int y, float startZ, float endZ, const vec3& color)
    {
        assert(startX <= endX && y >= 0 && y < m_height);
        startX = std::max(startX, 0);
        endX = std::min(endX, static_cast<int>(m_width));

        float zStep = (endZ-startZ)/(endX - startX);
        float z = startZ;
        for (int i = startX; i < endX; ++i)
        {
            putPixelDepth(i, y, z, color);
            z += zStep;
        }
    }

    void Context::putPixelRow(const vec3& start, const vec3& end, const vec3& color)
    {
        assert(start.y == end.y);
        putPixelRow(std::floor(start.x), std::ceil(end.x), start.y, color);
    }

    void Context::putPixelRowDepth(const vec3& start, const vec3& end, const vec3& color)
    {
        putPixelRowDepth(start.x, end.x, start.y, start.z, end.z, color);
    }

    int randomIntInRange(int min, int max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(min, max);
        return dis(gen);
    }

    float randomFloatInRange(float min, float max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }

    vec3 Context::castRay(const Ray& ray, int depth, vec3 pixelWorld, int xp, int yp) const
    {   
        if (depth > Ray::MAX_DEPTH)
        {
            return m_clearColor;
        }


        vec3 resultColor;

        auto [anyHit, hitPoint, hitPrimitive] = traceRay(ray);

        if (anyHit)
        {
            if (DOF && depth == 0)
            {
                /*float d1 = 1;
                float d2 = math::distance(ray.origin, pixelWorld);
                vec3 P = ray.origin + ((d2) / (d1 / (d1 + 900))) * ray.dir;

                if (math::distance(ray.origin, hitPoint) <= math::distance(ray.origin, P)) {

                    return vec3(1, 0, 0);
                }
                else {
                    return vec3(0);
                }*/

                vec3 accumulatedColor = vec3(0);
                float d1 = 1;
                float d2 = math::distance(ray.origin, pixelWorld);

                vec3 P = ray.origin + ((d2) / (d1 / (d1 + focalLength))) * ray.dir;

                int numOfAxis = (int)sqrt(apertureSize);
                float step = numOfAxis/2;

                for (int i = 0; i < numberOfRays; i++)
                {
                        float jitterX = randomIntInRange(-step, step);
                        float jitterY = randomIntInRange(-step, step);

                        //std::cout << apertureSize << ", " << numOfAxis << " | " << jitterX << " " << jitterY << std::endl;

                        vec4 pixelWorldSkewed = m_PVM.inverse() * vec4(xp + jitterX, yp + jitterY, -1, 1);
                        pixelWorldSkewed = pixelWorldSkewed / pixelWorldSkewed.w;

                        vec3 rayDir = math::normalize(P - vec3(pixelWorldSkewed.x, pixelWorldSkewed.y, pixelWorldSkewed.z));
                        Ray ray(pixelWorldSkewed, rayDir);
                        accumulatedColor += castRay(ray, depth + 1, pixelWorld, xp, yp);
                }

                return accumulatedColor / float(numberOfRays);
            }

            vec3 normal = hitPrimitive->getNormal(hitPoint);
            float ior = hitPrimitive->getMaterial().ior;
            
            if (ray.type == Ray::Type::INSIDE)
            {
                normal = -normal;
                ior = 1 / ior;
            }

            vec3 reflected = vec3(0);
            if (hitPrimitive->getMaterial().ks != 0) {
                vec3 reflectedDir = math::reflect(ray.dir, normal);
                reflected = hitPrimitive->getMaterial().ks * castRay(Ray(hitPoint, reflectedDir, ray.type), depth+1, pixelWorld, xp, yp);
            }

            vec3 refracted = vec3(0);
            if (hitPrimitive->getMaterial().T != 0) {
                vec3 refractedDir = math::refract( ray.dir, normal, ior);
                if (refractedDir != vec3())
                {
                    Ray::Type rayType = ray.type == Ray::Type::INSIDE ? Ray::Type::NORMAL : Ray::Type::INSIDE;
                    refracted = hitPrimitive->getMaterial().T * castRay(Ray(hitPoint + refractedDir * 0.0018, refractedDir, rayType), depth + 1, pixelWorld, xp, yp);
                }
            }
        
            resultColor = calculatePhong(hitPrimitive->getMaterial(), hitPoint, hitPrimitive->getNormal(hitPoint), math::normalize(vec3(ray.origin) - hitPoint));



            return resultColor + reflected + refracted;
        }
        return m_clearColor;
    }

    Context::TraceRayResult Context::traceRay(const Ray& cray, bool anyHit, float eps) const 
    {
        Ray ray = cray;

        std::shared_ptr<Primitive> closestPrimitive = nullptr;
        vec3 closestIntersection;

        float closestDistance = std::numeric_limits<float>::max();
        if (anyHit)
        {
            closestDistance = math::length(ray.dir) - 0.95;
        }
        ray.dir = math::normalize(ray.dir);
    
        for (const auto& primitive : m_scenePrimitives)
        {
            auto [isIntersected, point, t] = primitive->intersect(ray);
            if (isIntersected)
            {
                if (ray.type != Ray::Type::INSIDE && math::dotProduct(ray.dir, primitive->getNormal(point)) > 0)
                {
                    continue;
                }
                float distance = math::distance(ray.origin, point);
                if (distance < closestDistance)
                {
                    closestDistance = distance;
                    closestIntersection = point;
                    closestPrimitive = primitive;

                    if (anyHit)
                    {
                        break;
                    }
                }
            }
        }

        return { closestPrimitive != nullptr, closestIntersection, closestPrimitive };
    }

    void Context::beginPrimitive(uint32_t elementType) 
    {
        m_elementType = elementType;
        m_isDrawing = true;
        m_vertexBuffer.clear();
        updatePVM();
    }

    void Context::addVertex(const vec4& vertex) 
    {
        assert(m_isDrawing);
        if (!m_isSpecifyingScene)
        {
            vec4 transformed = m_PVM * vertex;
            m_vertexBuffer.push_back(transformed / transformed.w);
        }
        else 
        {
            m_vertexBuffer.push_back(vertex);
        }
    }

    void Context::addVertex(const vec4& vertex, const mat4& matrix) 
    {
        assert(m_isDrawing);
        m_vertexBuffer.push_back(matrix * vertex);
    }

    void Context::endPrimitive()
    {
        assert(m_elementType != SGL_LAST_ELEMENT_TYPE && m_isDrawing);
        if ((m_elementType > SGL_POINTS && m_vertexBuffer.size() < 2))
        {
            m_elementType = SGL_LAST_ELEMENT_TYPE;
            m_vertexBuffer.clear();
            return;
        }

        if (m_isSpecifyingScene)
        {
            switch (m_elementType)
            {
                case SGL_POLYGON:
                case SGL_TRIANGLES:
                {
                    assert(m_vertexBuffer.size() == 3);
                    const vec4& v1 = m_vertexBuffer[0];
                    const vec4& v2 = m_vertexBuffer[1];
                    const vec4& v3 = m_vertexBuffer[2];
                    m_scenePrimitives.emplace_back(std::make_shared<Triangle>(m_currentMaterial, v1, v2, v3));
                    break;
                }
                default:
                    assert(false && "Cannot have lines/points in a scene");
                    break;
            }
        }
        else 
        {
            drawBuffer();
        }

        m_isDrawing = false;
        m_elementType = SGL_LAST_ELEMENT_TYPE;
        m_vertexBuffer.clear();
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
                    switch (m_areaMode)
                    {
                        case SGL_POINT:
                        {
                            for (int i = 0; i < vertexCount; ++i)
                            {
                                vec3 p = m_vertexBuffer[i];
                                drawPoint(p);
                            }
                            break;
                        }
                        case SGL_LINE:
                        {
                            for (int i = 0; i < vertexCount; ++i)
                            {
                                vec3 p1 = m_vertexBuffer[i];
                                vec3 p2 = m_vertexBuffer[(i+1) % vertexCount];
                                drawLine(p1, p2);
                            }
                            break;
                        }
                        case SGL_FILL:
                        {
                            m_fillFunc(m_vertexBuffer);
                            break;
                        }
                    }
                }
                break;
            case SGL_TRIANGLES:
                switch (m_areaMode)
                {
                    case SGL_POINT:
                    {
                        for (int i = 2; i < vertexCount; ++i)
                        {
                            vec4 p1 = m_vertexBuffer[i-2];
                            vec4 p2 = m_vertexBuffer[i-1];
                            vec4 p3 = m_vertexBuffer[i];
                            drawPoint(p1);
                            drawPoint(p2);
                            drawPoint(p3);                         
                        }
                        break;
                    }
                    case SGL_LINE:
                    {
                        for (int i = 2; i < vertexCount; ++i)
                        {
                            vec4 p1 = m_vertexBuffer[i-2];
                            vec4 p2 = m_vertexBuffer[i-1];
                            vec4 p3 = m_vertexBuffer[i];
                            drawLine(p1, p2);
                            drawLine(p2, p3);
                            drawLine(p3, p1);    
                        }
                        break;
                    }
                    case SGL_FILL:
                    {
                        for (int i = 2; i < vertexCount; ++i)
                        {
                            vec4 p1 = m_vertexBuffer[i-2];
                            vec4 p2 = m_vertexBuffer[i-1];
                            vec4 p3 = m_vertexBuffer[i];
                            m_fillFunc({p1, p2, p3});
                        }
                        break;
                    }
                }
                
            default:
                assert(false); // Not implemented
                break;
        }
    }

    void Context::beginScene()
    {
        m_isSpecifyingScene = true;
        m_scenePrimitives.clear();
        m_sceneLights.clear();
    }

    void Context::endScene()
    {
        m_isSpecifyingScene = false;
    }

    bool Context::isSpecifyingScene() const
    {
        return m_isSpecifyingScene;
    }

    void Context::renderScene()
    {
        std::shared_ptr<Light> directional = std::make_shared<DirectionalLight>(vec3(-1, -2, 3), vec3(1));
        // addLight(directional);

        // PointLight* pl = dynamic_cast<PointLight*>(m_sceneLights[0].get());
        // pl->m_pos.y -= 300;

        // setMatrixMode(SGL_MODELVIEW);
        // getCurrentMat() *= translate(0,0,250);

        mat4 invModelView = getModelView().inverse();
        mat4 invPVM = m_PVM.inverse();

        vec4 originWorld = invModelView * vec4(0, 0, 0, 1);

        for (int yp = 0; yp < m_height; ++yp)
        {
            for (int xp = 0; xp < m_width; ++xp)
            {
                vec4 pixelWorld = invPVM * vec4(xp, yp, -1, 1);
                pixelWorld = pixelWorld / pixelWorld.w;

                vec3 rayDir = math::normalize(vec3(pixelWorld) - vec3(originWorld));
                Ray primary(originWorld, rayDir);

                vec3 color = castRay(primary, 0, vec3(pixelWorld.x, pixelWorld.y, pixelWorld.z), xp, yp);

                putPixel(vec3(xp, yp, 0), color);
            }
        }
    }

    void Context::setCurrentMaterial(const Material& material)
    {
        m_currentMaterial = material;
    }

    void Context::addLight(std::shared_ptr<Light> light)
    {
        m_sceneLights.push_back(light);
    }

    vec3 Context::calculatePhong(const Material& material, const vec3& intersectionPoint, const vec3& surfaceNormal, const vec3& camera) const
    {
        vec3 result(0.0f, 0.0f, 0.0f);    

        //scene light iteration
        for (std::shared_ptr<Light> light : m_sceneLights)
        {
            vec3 diffuse(0.0f, 0.0f, 0.0f);  
            vec3 specular(0.0f, 0.0f, 0.0f); 
            //casting

            vec3 lightDir = light->getDirection(intersectionPoint);

            Ray lightRay(intersectionPoint + lightDir * 0.0001, lightDir);
            auto [anyHit, a, b] =  traceRay(lightRay, true);

            if (anyHit)
            {
                continue;
            }

            lightDir = math::normalize(lightDir);
            vec3 reflectedDir = math::normalize(surfaceNormal * (2.0f * math::dotProduct(surfaceNormal, lightDir)) - lightDir);

            float diff = std::fmax(0.0f, math::dotProduct(surfaceNormal, lightDir));

            diffuse = (light->getColor() * material.color * material.kd * diff);

            float spec = std::pow(std::fmax(0.0f, math::dotProduct(camera, reflectedDir)), material.shine);

            specular = (light->getColor() * material.ks * spec);

            result += (diffuse + specular);
        }

        return result;
    }

    void Context::addSphere(const vec3 &center, float radius)
    {
        m_scenePrimitives.emplace_back(std::make_shared<Sphere>(m_currentMaterial, center, radius));
    }

    void Context::setAreaMode(uint32_t areaMode)
    {
        m_areaMode = areaMode;
    }

    void Context::drawCircle(vec3 center, float radius, bool fill) 
    {
        vec3 tCenter(m_PVM * vec4(center, 1));

        if (m_areaMode == SGL_POINT)
        {
            drawPoint(tCenter);
            return;
        }

        vec3i c = tCenter;

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

        std::function putLineFunc = [&](const vec3& p1, const vec3& p2, const vec3& color) { putPixelRow(p1, p2, color); };
        std::function putPixelFunc = [&](const vec3& p, const vec3& color) { putPixel(p, color); };
        if (m_features.test(SGL_DEPTH_TEST))
        {
            putLineFunc = [&](const vec3& p1, const vec3& p2, const vec3& color) { putPixelRowDepth(p1, p2, color); };
            putPixelFunc = [&](const vec3& p, const vec3& color) { putPixelDepth(p, color); };
        }

        std::function putPixels = [&]() { 
            putPixelFunc(vec3( x+c.x,  y+c.y, tCenter.z), m_drawColor);
            putPixelFunc(vec3( x+c.x, -y+c.y, tCenter.z), m_drawColor);
            putPixelFunc(vec3(-x+c.x,  y+c.y, tCenter.z), m_drawColor);
            putPixelFunc(vec3(-x+c.x, -y+c.y, tCenter.z), m_drawColor);
            putPixelFunc(vec3( y+c.x,  x+c.y, tCenter.z), m_drawColor);
            putPixelFunc(vec3( y+c.x, -x+c.y, tCenter.z), m_drawColor);
            putPixelFunc(vec3(-y+c.x,  x+c.y, tCenter.z), m_drawColor);
            putPixelFunc(vec3(-y+c.x, -x+c.y, tCenter.z), m_drawColor);
        };
        if (m_areaMode == SGL_FILL)
        {
            putPixels = [&]() {
                putLineFunc(vec3(tCenter.x-x, c.y + y, c.z), vec3(tCenter.x + x, c.y + y, c.z), m_drawColor);
                putLineFunc(vec3(tCenter.x-x, c.y - y, c.z), vec3(tCenter.x + x, c.y - y, c.z), m_drawColor);
                putLineFunc(vec3(tCenter.x-y, c.y + x, c.z), vec3(tCenter.x + y, c.y + x, c.z), m_drawColor);
                putLineFunc(vec3(tCenter.x-y, c.y - x, c.z), vec3(tCenter.x + y, c.y - x, c.z), m_drawColor);
            };
        }

        while (x <= y)
        {
            putPixels();
            if (p <= 0)
            {
                p += fourX + 6;
            }
            else 
            {
                p += (fourX - fourY) + 10;
                fourY -= 4;
                --y;
            }
            fourX += 4;
            ++x;
        }
    }

    void Context::drawCirclePolar(vec3 center, float radius)
    {
        int steps = 40;
        float dtheta = 2 * M_PI / steps;

        vec3i c(center);
        beginPrimitive(SGL_LINE_LOOP);
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
        beginPrimitive(m_areaMode == SGL_FILL ? SGL_POLYGON : SGL_LINE_LOOP);
        for (int i = 0; i < steps; ++i)
        {
            float theta = i * dtheta;
            float x = center.x + a * std::cos(theta);
            float y = center.y + b * std::sin(theta);
            addVertex(vec4(x, y, center.z, 1));
        }
        endPrimitive();
    }

    void Context::drawArc(vec3 center, float radius, float fromRad, float toRad)
    {    
        int steps = 40;

        steps = static_cast<int>( static_cast<float>(steps) * (toRad-fromRad) / 2 * M_PI);

        float dtheta = (toRad-fromRad) / steps;

        bool isFilled = m_areaMode == SGL_FILL;
        beginPrimitive(isFilled ? SGL_POLYGON : SGL_LINE_STRIP);
        for (int i = 0; i < steps; ++i)
        {
            float theta = fromRad + i * dtheta;
            float x = center.x + radius * std::cos(theta);
            float y = center.y + radius * std::sin(theta);
            addVertex(vec4(x, y, center.z, 1));
        }
        if (isFilled)
        {
            addVertex(vec4(center, 1));
        }
        endPrimitive();

    }

    void Context::drawPoint(float x, float y, float z) 
    {
        bool isDepthTest = m_features.test(SGL_DEPTH_TEST);
        float halfSize = m_pointSize * 0.5;

        if (isDepthTest)
        {
            assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
            int ix = static_cast<int>(x);
            int iy = static_cast<int>(y);
            int idx = point2idx(ix, iy);
            if (m_depthBuffer[idx] < z)
            {
                return;
            }
            else
            {
                m_depthBuffer[idx] = z;
            }
        }

        for (int yPixel = std::max(static_cast<int>(y-halfSize), 0); yPixel < std::min(static_cast<uint32_t>(y+halfSize), m_height); ++yPixel)
        {
            uint32_t xPixelStart = std::max(static_cast<int>(x-halfSize), 0);
            uint32_t xPixelEnd = x + halfSize;

            putPixelRow(xPixelStart, xPixelEnd, yPixel, m_drawColor);
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
            vec3 p1 = vertices[i];
            vec3 p2 = vertices[(i+1) % vertices.size()];

            if (static_cast<int>(p1.y) == static_cast<int>(p2.y))
            {
                continue;
            }

            if (p1.y > p2.y)
            {
                std::swap(p1, p2);
            }

            Edge e;
            e.yMax = static_cast<int>(p2.y);
            e.x = p1.x;
            e.inverseSlope = (p2.x-p1.x) / (p2.y-p1.y);
            edgeTable[static_cast<int>(p1.y) - minY].emplace_back(e);
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
                int startX = std::floor(activeTable[i-1].x);
                int endX = std::ceil(activeTable[i].x);
                putPixelRow(startX, endX, y, m_drawColor);
            }

            for (auto& edge : activeTable)
            {
                edge.x += edge.inverseSlope;
            }
        }
    }

    void Context::fillDepth(const std::vector<vec4>& vertices)
    {
        struct Edge
        {
            int yMax;
            float x;
            float inverseSlope;
            float z;
            float zSlope;
        };

        auto yComparator = [](const auto& v1, const auto& v2) { return v1.y < v2.y; };
        auto [min, max] = std::minmax_element(vertices.begin(), vertices.end(), yComparator);
        assert(min != vertices.end() && max != vertices.end());
        int minY = (*min).y;
        int maxY = (*max).y;

        std::vector<std::vector<Edge>> edgeTable(maxY - minY + 1);

        for (int i = 0; i < vertices.size(); ++i)
        {
            vec3 p1 = vertices[i];
            vec3 p2 = vertices[(i+1) % vertices.size()];

            if (static_cast<int>(p1.y) == static_cast<int>(p2.y))
            {
                continue;
            }

            if (p1.y > p2.y)
            {
                std::swap(p1, p2);
            }

            Edge e;
            e.yMax = static_cast<int>(p2.y);
            e.x = p1.x;
            e.inverseSlope = (p2.x-p1.x) / (p2.y-p1.y);
            e.z = p1.z;
            e.zSlope = (p2.z-p1.z) / (p2.y-p1.y);
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
                int startX = std::floor(activeTable[i-1].x);
                int endX = std::ceil(activeTable[i].x);

                float startZ = activeTable[i-1].z;
                float endZ = activeTable[i].z;
                
                putPixelRowDepth(startX, endX, y, startZ, endZ, m_drawColor);
            }

            for (auto& edge : activeTable)
            {
                edge.x += edge.inverseSlope;
                edge.z += edge.zSlope;
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
