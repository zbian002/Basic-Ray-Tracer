#include "refractive_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Refractive_Shader::
Shade_Surface(const Ray& ray, const vec3& intersection_point,
        const vec3& same_side_normal, int recursion_depth,bool is_exiting) const
{
    vec3 reflection_color;
    vec3 refraction_color;
    double reflectance_ratio=-1;
    if(!world.disable_fresnel_refraction)
    { 
        double ni, nr;
        if(is_exiting == true) {
            ni = this->refractive_index;
            nr = REFRACTIVE_INDICES::AIR;
        }
        else {
            ni = REFRACTIVE_INDICES::AIR;
            nr = this->refractive_index;
        }
        double cosi = dot(same_side_normal, -ray.direction);
        double cosr2 = 1 - (pow((ni / nr), 2.0) * (1 - pow(cosi, 2.0)));
	if(cosr2 < 0) 
            reflectance_ratio = 1;
	else {
            Ray refracted_ray;
            double cosr = sqrt(cosr2);
            double r1 = pow(((nr * cosi - ni * cosr) / (nr * cosi + ni * cosr)), 2.0);
            double r2 = pow(((ni * cosi - nr * cosr) / (ni * cosi + nr * cosr)), 2.0);
            reflectance_ratio = (r1 + r2) / 2;
	    vec3 T = ((ni / nr) * (ray.direction - dot(ray.direction, same_side_normal) * same_side_normal) - cosr * same_side_normal).normalized();
	    refracted_ray.endpoint = intersection_point;
	    refracted_ray.direction = T;
	    refraction_color = world.Cast_Ray(refracted_ray, recursion_depth + 1);
        }
    }
    if(!world.disable_fresnel_reflection){
        //TODO:(Test 26+): Compute reflection_color:
	Ray reflected_ray;
        vec3 r = (ray.direction - 2 * dot(ray.direction, same_side_normal) * same_side_normal).normalized();
        reflected_ray.endpoint = intersection_point;
        reflected_ray.direction = r;
        reflection_color = world.Cast_Ray(reflected_ray, recursion_depth + 1);
    }

    Enforce_Refractance_Ratio(reflectance_ratio);
    vec3 color;
    // TODO: (Test 26+) Compute final 'color' by blending reflection_color and refraction_color using 
    color = reflectance_ratio * reflection_color + (1 - reflectance_ratio) * refraction_color;
    return color;
}

void Refractive_Shader::
Enforce_Refractance_Ratio(double& reflectance_ratio) const
{
    if(world.disable_fresnel_reflection) reflectance_ratio=0;
    else if(world.disable_fresnel_refraction) reflectance_ratio=1;
}

