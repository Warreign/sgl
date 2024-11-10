#include "sgl.h"

#include "context/context_manager.h"
#include "context/context.h"

#include "math/vector.h"
#include "math/matrix.h"
#include "math/transform.h"

#include <cassert>
#include <iostream>

#pragma GCC diagnostic ignored "-Wsign-compare"


sgl::SglController sgl::SglController::s_instance;

void sglInit(void)
{
    std::cout << "Hello SGL!" << std::endl;
}

sglEErrorCode sglGetError(void)
{
    return static_cast<sglEErrorCode>(sgl::SglController::getInstance().getError());
}

const char *sglGetErrorString(sglEErrorCode error)
{
    return sgl::SglController::getInstance().getErrorString(error);
}

void sglFinish(void)
{
}

int sglCreateContext(int width, int height)
{
    assert(width > 0 && height > 0);
    return sgl::SglController::getInstance().createContext(width, height);
}

void sglDestroyContext(int id)
{
    sgl::SglController::getInstance().destroyContext(id);
}

void sglSetContext(int id)
{
    sgl::SglController::getInstance().setActive(id);
}

int sglGetContext(void)
{
    return sgl::SglController::getInstance().getActiveId();
}

float *sglGetColorBufferPointer(void)
{
    sgl::Context* context = sgl::SglController::getInstance().getActive();
    return context ? context->colorBufferData() : nullptr;
}

void sglClear(unsigned what)
{
    sgl::SglController& m = sgl::SglController::getInstance();
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
    sgl::SglController& m = sgl::SglController::getInstance();
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
    sgl::SglController& m = sgl::SglController::getInstance();
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
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context) { return; }
    context->addVertex(sgl::vec4(x, y, z, w));
}

void sglVertex3f(float x, float y, float z)
{
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context) { return; }
    context->addVertex(sgl::vec4(x, y, z, 1));
}

void sglVertex2f(float x, float y)
{    
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context) { return; }
    context->addVertex(sgl::vec4(x, y, 0, 1));
}

void sglCircle(float x, float y, float z, float radius)
{
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }

    context->drawCircle(sgl::vec3(x, y, z), radius);
}

void sglEllipse(float x, float y, float z, float a, float b)
{
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }

    context->drawEllipse(sgl::vec3(x, y, z), a, b);
}

void sglArc(float x, float y, float z, float radius, float from, float to)
{
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }

    context->drawArc(sgl::vec3(x,y, z), radius, from, to);
}

void sglMatrixMode(sglEMatrixMode mode)
{
    sgl::SglController& m = sgl::SglController::getInstance();
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
    sgl::SglController& m = sgl::SglController::getInstance();
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
    sgl::SglController& m = sgl::SglController::getInstance();
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
    context->updatePVM();
}

void sglLoadIdentity(void)
{
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    current = sgl::mat4::identity;
    context->updatePVM();
}

void sglLoadMatrix(const float *matrix)
{
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    sgl::mat4 newMatrix(matrix);
    current = newMatrix;
    context->updatePVM();
}

void sglMultMatrix(const float *matrix)
{
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    sgl::mat4 sglMatrix(matrix);
    current = current * sglMatrix;
    context->updatePVM();
}

void sglTranslate(float x, float y, float z)
{
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    sgl::mat4 translation = sgl::translate(x, y, z);
    current = current * translation;
    context->updatePVM();
}

void sglScale(float scalex, float scaley, float scalez)
{
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    current = current * sgl::scale(scalex, scaley, scalez);;
    context->updatePVM();
}

void sglRotate2D(float angle, float centerx, float centery)
{
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    current = current * sgl::translate(centerx, centery, 0) * sgl::rotateZ(angle) * sgl::translate(-centerx, -centery, 0);
    context->updatePVM();
}

void sglRotateY(float angle)
{
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    sgl::mat4& current = context->getCurrentMat();
    current = current * sgl::rotateY(angle);
    context->updatePVM();
}

void sglOrtho(float left, float right, float bottom, float top, float near, float far)
{   
    sgl::SglController& m = sgl::SglController::getInstance();
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
    current = current * sgl::ortho(left, right, bottom, top, near, far);
    context->updatePVM();
}

void sglFrustum(float left, float right, float bottom, float top, float near, float far)
{
    sgl::SglController& m = sgl::SglController::getInstance();
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
    current = current * sgl::perspective(left, right, bottom, top, near, far);
    context->updatePVM();
}

void sglViewport(int x, int y, int width, int height)
{    
    sgl::SglController& m = sgl::SglController::getInstance();
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
    context->updatePVM();
}

void sglClearColor(float r, float g, float b, float alpha)
{
    sgl::SglController& m = sgl::SglController::getInstance();
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
    sgl::SglController& m = sgl::SglController::getInstance();
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
    sgl::SglController& m = sgl::SglController::getInstance();
    sgl::Context* context = m.getActive();
    if (!context || context->isDrawing())
    {
        m.setError(SGL_INVALID_OPERATION);
        return;
    }
    if (mode > SGL_FILL)
    {
        m.setError(SGL_INVALID_ENUM);
        return;
    }
    context->setAreaMode(mode);
}

void sglPointSize(float size)
{
    sgl::SglController& m = sgl::SglController::getInstance();
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
    sgl::SglController& m = sgl::SglController::getInstance();
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
    sgl::SglController& m = sgl::SglController::getInstance();
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
