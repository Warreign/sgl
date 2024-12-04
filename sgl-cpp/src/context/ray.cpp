#include "ray.h"
#include "math/utils.h"

namespace sgl
{

Ray::Ray(const vec3& origin, const vec3& dir)
	: origin(origin), dir(math::normalize(dir))
{
}

Ray::Ray(float ox, float oy, float oz, float dx, float dy, float dz)
	: origin(ox, oy, oz), dir(math::normalize(vec3(dx, dy, dz)))
{
}

}

