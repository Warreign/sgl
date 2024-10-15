#include "sgl.h"

#include "context/context.h"
#include "context/context_manager.h"

#include "math/matrix.h"
#include "math/transform.h"
#include "math/utils.h"

#include <stack>
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
}

void sglBegin(sglEElementType mode)
{
}

void sglEnd(void)
{
}

void sglVertex4f(float x, float y, float z, float w)
{
}

void sglVertex3f(float x, float y, float z)
{
}

void sglVertex2f(float x, float y)
{
}

void sglCircle(float x, float y, float z, float radius)
{
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
    sgl::mat4 scale = sgl::scale(scalex, scaley, scalez);
    current *= scale;
}

void sglRotate2D(float angle, float centerx, float centery)
{
}

void sglRotateY(float angle)
{
}

void sglOrtho(float left, float right, float bottom, float top, float, float)
{
}

void sglFrustum(float left, float right, float bottom, float top, float, float)
{
}

void sglViewport(int x, int y, int width, int height)
{
}

void sglClearColor(float r, float g, float b, float alpha)
{
}

void sglColor3f(float r, float g, float b)
{
}

void sglAreaMode(sglEAreaMode mode)
{
}

void sglPointSize(float size)
{
}

void sglEnable(sglEEnableFlags cap)
{
}

void sglDisable(sglEEnableFlags cap)
{
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
