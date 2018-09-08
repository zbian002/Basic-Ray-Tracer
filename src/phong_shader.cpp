#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"
#include <algorithm>

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const 
{
    vec3 color;
    // TODO: determine the color
    Hit hit;
    color = world.ambient_color * world.ambient_intensity * color_ambient;
    for(unsigned i = 0; i < world.lights.size(); i++) {
        vec3 l = world.lights.at(i)->position - intersection_point;
        if(world.enable_shadows) {
            Ray temp;
            temp.endpoint = intersection_point;
            temp.direction = l.normalized();
            if(world.Closest_Intersection(temp, hit)) {
		if(hit.t < l.magnitude())
			continue;
            }
        }
        vec3 r = (2 * dot(l, same_side_normal) * same_side_normal - l).normalized();
        vec3 light_color = world.lights.at(i)->Emitted_Light(ray) / (l.magnitude_squared());
        double diffuse_intensity = std::max(0.0, dot(l.normalized(), same_side_normal));
        double specular_intensity = pow(std::max(0.0, dot(r, (ray.endpoint - intersection_point).normalized())), specular_power);
	color = light_color * (color_diffuse * diffuse_intensity + color_specular * specular_intensity) + color;
    }
    return color;
}
