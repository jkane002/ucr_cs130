#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth) const
{
    vec3 temp, lightvec, color;
    vec3 R;
    lightvec = (ray.endpoint - intersection_point).normalized();
    R = (2 * dot(lightvec.normalized(), same_side_normal) *
                same_side_normal - lightvec.normalized() );
    Ray r;
    r.endpoint = intersection_point;
    r.direction = R;
    
    temp = world.Cast_Ray(r,recursion_depth+1);
    color += temp * reflectivity + (1 - reflectivity) *
        shader->Shade_Surface(ray, intersection_point, same_side_normal, recursion_depth);
    return color;
}
