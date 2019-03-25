#include "plane.h"
#include "ray.h"
#include <cfloat>


// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
bool Plane::
Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    double t = dot(this->normal, (this->x1 - ray.endpoint)) /
            (dot(this->normal, ray.direction));

    if(t >= 0.0) {
        Hit temp = {this, t, true};
        hits.push_back(temp);
        return true;
    } else
        return false;
}

vec3 Plane::
Normal(const vec3& point) const
{
    return normal;
}
