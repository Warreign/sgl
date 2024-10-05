#include "sgl.h"

#include <iostream>

void sglInit(void)
{
    std::cout << "Hello SGL!" << std::endl;
}

sglEErrorCode sglGetError(void)
{
    return sglEErrorCode();
}

const char *sglGetErrorString(sglEErrorCode error)
{
    return nullptr;
}

void sglFinish(void)
{
}

int sglCreateContext(int width, int height)
{
    return 0;
}

void sglDestroyContext(int id)
{
}

void sglSetContext(int id)
{
}

int sglGetContext(void)
{
    return 0;
}

float *sglGetColorBufferPointer(void)
{
    return nullptr;
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
}

void sglPushMatrix(void)
{
}

void sglPopMatrix(void)
{
}

void sglLoadIdentity(void)
{
}

void sglLoadMatrix(const float *matrix)
{
}

void sglMultMatrix(const float *matrix)
{
}

void sglTranslate(float x, float y, float z)
{
}

void sglScale(float scalex, float scaley, float scalez)
{
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
