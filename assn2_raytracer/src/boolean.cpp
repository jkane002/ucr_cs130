#include "boolean.h"


// Determine if the ray intersects with the boolean of A and B.
bool Boolean::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    if(type == type_intersection) {
    }
    else if (type == type_union) {
        bool obj1 = false;
        bool obj2 = false;
        obj1 = A->Intersection(ray,hits);
        // obj2 = B->Intersection(ray,hits);

        if(obj1 && obj2)
            return true;
        else
            return false;
    }
    else if (type == type_difference) {
    }
    else {
        std::cout << "in boolean type: ERROR\n";
    }
}

// This should never be called.
vec3 Boolean::Normal(const vec3& point) const
{
    assert(false);
    return vec3();
}
