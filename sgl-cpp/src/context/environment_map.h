#pragma once

namespace sgl
{

    struct EnvironmentMap
    {
        EnvironmentMap() = default;

        EnvironmentMap(int width, int height, float* texels) :
            width(width), height(height), texels(texels)
        {
        }

        int width;
        int height;
        float* texels;
    };

}