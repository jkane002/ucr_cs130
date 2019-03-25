#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"


vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth) const
{
    vec3 color;
    vec3 dif;
    vec3 spec;
    vec3 amb;
    vec3 R;
    //R=(2∗(L⋅N)∗N−L)
    // ambient
    amb = world.ambient_color * world.ambient_intensity * color_ambient;
    color += amb;

    vec3 lightvec;

    for(unsigned i = 0; i < world.lights.size(); i++) {
    //dif
        lightvec = world.lights.at(i)->position - intersection_point;
        if(world.enable_shadows) {
            Ray rayShadow;
            rayShadow.endpoint = intersection_point + lightvec.normalized();
            rayShadow.direction = lightvec.normalized();
            Hit hit1;
            Object* obj1 = world.Closest_Intersection(rayShadow, hit1);
            bool cond = dot(world.lights.at(i)->position - rayShadow.Point(hit1.t), rayShadow.direction) > 0;
            if(obj1 != nullptr && cond) {
                continue;
            }
        }

        dif += fmax(dot(same_side_normal, lightvec.normalized() ) , 0 ) * color_diffuse
                * (world.lights.at(i)->Emitted_Light(ray)/
                    pow(lightvec.magnitude(), 2 ));

        //spec
        R = (2 * dot(lightvec.normalized(), same_side_normal) *
                    same_side_normal - lightvec.normalized() );

        spec += pow( fmax(dot(R.normalized(),(ray.endpoint - intersection_point).normalized() ),0), specular_power)
                * color_specular * (world.lights.at(i)->Emitted_Light(ray)/ pow(lightvec.magnitude(), 2 ));
    }
    color += dif;
    color += spec;
    return color;
}
