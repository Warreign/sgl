#include "ray.h"
#include "math/utils.h"

sgl::Ray::Ray(const vec3& origin, vec3& dir)
	: origin(origin), dir(dir)
{
	dir = sgl::math::normalize(dir);
}

sgl::Ray::Ray(float ox, float oy, float oz, float dx, float dy, float dz)
	: origin(ox, oy, oz), dir(dx, dy, dz)
{
	dir = sgl::math::normalize(dir);
}


