#include "ray.h"
#include "math/utils.h"

namespace sgl
{

Ray::Ray(const vec3& origin, const vec3& dir, Type type)
	: origin(origin), dir(math::normalize(dir)), type(type)
{
}

Ray::Ray(float ox, float oy, float oz, float dx, float dy, float dz, Type type)
	: origin(ox, oy, oz), dir(math::normalize(vec3(dx, dy, dz))), type(type)
{
}

}

