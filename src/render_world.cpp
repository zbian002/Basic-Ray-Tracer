#include <vector>
#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"


Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3),disable_fresnel_reflection(false),disable_fresnel_refraction(false)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find the closest object of intersection and return the object that was
// intersected.  Record the Hit structure in hit.  If no intersection occurred,
// return NULL.  Note that in the case of a Boolean, the object returned will be
// the Boolean, but the object stored in hit will be the underlying primitive.
// Any intersection with t<=small_t should be ignored.
Object* Render_World::Closest_Intersection(const Ray& ray,Hit& hit)
{
    // TODO
    double min_t = std::numeric_limits<double>::max();
    Object* closest_object = NULL;
    for(unsigned i = 0; i < objects.size(); i++) {
        std::vector<Hit> hits;
        if(!objects.at(i)->Intersection(ray, hits))
            continue;
        for(unsigned j = 0; j < hits.size(); ++j) {
            if(hits.at(j).t < min_t && hits.at(j).t > small_t) {
                 closest_object = objects.at(i);
                 hit = hits.at(j);
                 min_t = hits.at(j).t;
            }
        }
    }
    return closest_object;
    //return 0;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    Ray ray; // TODO: set up the initial view ray here
    ray.endpoint = this->camera.position;
    vec3 w = this->camera.World_Position(pixel_index);
    ray.direction = (w - ray.endpoint).normalized();
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    // TODO
    vec3 color;
    Hit hit;
    Object* cobject = Closest_Intersection(ray, hit);
    // determine the color here
    vec3 dummy;
    if(recursion_depth > recursion_depth_limit) 
        return background_shader->Shade_Surface(ray, dummy, dummy, 1, false); 
    if(cobject != NULL) {
        vec3 intersection = ray.Point(hit.t);
        vec3 normal = cobject->Normal(intersection);
        if(hit.ray_exiting == true)
            normal = -normal;
        color = cobject->material_shader->Shade_Surface(ray, intersection, normal, recursion_depth, hit.ray_exiting);
    }
    else 
        color = background_shader->Shade_Surface(ray, dummy, dummy, 1, false); 
    return color;
}
