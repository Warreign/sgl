#include "ray.h"

namespace sgl
{

Ray::Ray(const vec3& direction)
    : dir(direction)
{
   
}

Ray::Ray(float x, float y, float z)
    : dir(x, y, z)
{
    
}

} // namespace sgl
