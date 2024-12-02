#include "ray.h"
#include "math/utils.h"

sgl::Ray::Ray(const vec3& origin, const vec3& dir)
	: origin(origin), dir(sgl::math::normalize(dir))
{
}

sgl::Ray::Ray(float ox, float oy, float oz, float dx, float dy, float dz)
	: origin(ox, oy, oz), dir(sgl::math::normalize(vec3(dx, dy, dz)))
{
}


