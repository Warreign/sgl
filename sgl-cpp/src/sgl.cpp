#include "sgl.h"

#include "context/context_manager.h"
#include "context/context.h"

#include "math/vector.h"
#include "math/matrix.h"
#include "math/transform.h"
#include "math/utils.h"

#include <cassert>
#include <iostream>


sgl::ContextManager sgl::ContextManager::s_instance;

void sglInit(void)
{
    std::cout << "Hello SGL!" << std::endl;
}

sglEErrorCode sglGetError(void)
{
    return static_cast<sglEErrorCode>(sgl::ContextManager::getInstance().getError());
}

const char *sglGetErrorString(sglEErrorCode error)
{
    return sgl::ContextManager::getInstance().getErrorString(error);
}

void sglFinish(void)
{
}

int sglCreateContext(int width, int height)
{
    assert(width > 0 && height > 0);
    return sgl::ContextManager::getInstance().createContext(width, height);
}

void sglDestroyContext(int id)
{
    sgl::ContextManager::getInstance().destroyContext(id);
}

void sglSetContext(int id)
{
    sgl::ContextManager::getInstance().setActive(id);
}

int sglGetContext(void)
{
    return sgl::ContextManager::getInstance().getActiveId();
}

float *sglGetColorBufferPointer(void)
{
    sgl::Context* context = sgl::ContextManager::getInstance().getActive();
    return context ? context->colorBufferData() : nullptr;
}

void sglClear(unsigned what)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    context->clearBuffers(what);
}

void sglBegin(sglEElementType mode)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    if (!(mode >= SGL_POINTS && mode < SGL_LAST_ELEMENT_TYPE))
    {
        m.setError(SGL_INVALID_ENUM);
    }
    context->beginDrawing(mode);
}

void sglEnd(void)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context)
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    context->drawBuffer();
}

void sglVertex4f(float x, float y, float z, float w)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context) { return; }
    context->addVertex(sgl::vec4(x, y, z, w));
}

void sglVertex3f(float x, float y, float z)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context) { return; }
    context->addVertex(sgl::vec4(x, y, z, 1));
}

void sglVertex2f(float x, float y)
{    
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context) { return; }
    context->addVertex(sgl::vec4(x, y, 0, 1));
}

void sglCircle(float x, float y, float z, float radius)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }

    context->drawCircle(sgl::vec2i(x, y), radius);
}

void sglEllipse(float x, float y, float z, float a, float b)
{
}

void sglArc(float x, float y, float z, float radius, float from, float to)
{
}

void sglMatrixMode(sglEMatrixMode mode)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    switch (mode)
    {
        case SGL_MODELVIEW:
        case SGL_PROJECTION:
            context->setMatrixMode(mode);
            break;
        default:
            m.setError(SGL_INVALID_ENUM);
    }
}

void sglPushMatrix(void)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    auto& stack = context->getCurrentStack();    
    if (stack.size() == 100)
    {
        m.setError(SGL_STACK_OVERFLOW);
        return;
    }
    stack.push_back(stack.back());
}

void sglPopMatrix(void)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    auto& stack = context->getCurrentStack();    
    if (stack.size() == 1)
    {
        m.setError(SGL_STACK_UNDERFLOW);
        return;
    }
    stack.pop_back();
}

void sglLoadIdentity(void)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    current = sgl::mat4::identity;
}

void sglLoadMatrix(const float *matrix)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    sgl::mat4 newMatrix(matrix);
    current = newMatrix;
}

void sglMultMatrix(const float *matrix)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    sgl::mat4 sglMatrix(matrix);
    current *= sglMatrix;
}

void sglTranslate(float x, float y, float z)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    sgl::mat4 translation = sgl::translate(x, y, z);
    current *= translation;
}

void sglScale(float scalex, float scaley, float scalez)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    current *= sgl::scale(scalex, scaley, scalez);;
}

void sglRotate2D(float angle, float centerx, float centery)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    current = sgl::translate(centerx, centery, 0) * sgl::rotateZ(angle) * sgl::translate(-centerx, -centery, 0) * current;
}

void sglRotateY(float angle)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    current *= sgl::rotateY(angle);
}

void sglOrtho(float left, float right, float bottom, float top, float near, float far)
{   
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    if (left == right || bottom == top || near == far)
    {
        m.setError(SGL_INVALID_VALUE);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    current *= sgl::ortho(left, right, bottom, top, near, far);
}

void sglFrustum(float left, float right, float bottom, float top, float near, float far)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    if (left == right || bottom == top || near == far || near < 0 || far < 0)
    {
        m.setError(SGL_INVALID_VALUE);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    current *= sgl::perspective(left, right, bottom, top, near, far);
}

void sglViewport(int x, int y, int width, int height)
{    
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    if (width < 0 || height < 0)
    {
        m.setError(SGL_INVALID_VALUE);
        return;
    }
    context->setViewport(x, y, width, height);
}

void sglClearColor(float r, float g, float b, float alpha)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    context->setClearColor(sgl::vec3(r, g, b));
}

void sglColor3f(float r, float g, float b)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    context->setDrawColor(sgl::vec3(r, g, b));
}

void sglAreaMode(sglEAreaMode mode)
{
}

void sglPointSize(float size)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    context->setPointSize(size);
}

void sglEnable(sglEEnableFlags cap)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    context->enableFeatures(static_cast<uint32_t>(cap));
}

void sglDisable(sglEEnableFlags cap)
{
    sgl::ContextManager& m = sgl::ContextManager::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    context->disableFeatures(static_cast<uint32_t>(cap));
}

void sglBeginScene()
{
}

void sglEndScene()
{
}

void sglSphere(const float x, const float y, const float z, const float radius)
{
}

void sglMaterial(const float r, const float g, const float b, const float kd, const float ks, const float shine, const float T, const float ior)
{
}

void sglPointLight(const float x, const float y, const float z, const float r, const float g, const float b)
{
}

void sglRayTraceScene()
{
}

void sglRasterizeScene()
{
}

void sglEmissiveMaterial(const float r, const float g, const float b, const float c0, const float c1, const float c2)
{
}

void sglEnvironmentMap(const int width, const int height, float *texels)
{
}
