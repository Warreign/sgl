#include "ray.h"

namespace sgl
{

Ray::Ray(const vec3& origin, const vec3& dir, Type type)
	: origin(origin), dir(dir), type(type)
{
}

Ray::Ray(float ox, float oy, float oz, float dx, float dy, float dz, Type type)
	: origin(ox, oy, oz), dir(vec3(dx, dy, dz)), type(type)
{
}

}

