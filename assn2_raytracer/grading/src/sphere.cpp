#include "sphere.h"
#include "ray.h"


// Determine if the ray intersects with the sphere
bool Sphere::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    vec3 w = ray.direction;
    vec3 u = ray.endpoint;
    vec3 v = u - this->center;

    double discrim = (dot(w,v) * dot(w,v)) - (dot(v,v) - pow(this->radius, 2));
    double t0 = -(dot(w,v)) - sqrt(discrim);
    double t1 = -(dot(w,v)) + sqrt(discrim);

    Hit temp1 = {this, t0, true};
    Hit temp2 = {this, t1, true};

    if(t0 >= 0.0)
        hits.push_back(temp1);
    if(t1 >= 0.0)
        hits.push_back(temp2);
    if(discrim >= 0) {
        return true;
    } else {
        return false;
    }
}

vec3 Sphere::Normal(const vec3& point) const
{
    vec3 normal;
    // TODO: set the normal
    normal = ((point-center)/radius).normalized();
    return normal;
}
