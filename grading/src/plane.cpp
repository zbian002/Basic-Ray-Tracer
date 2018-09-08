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
    if(dot(ray.direction, this->normal) != 0) {
        double tplane = (dot(this->x1 - ray.endpoint, this->normal) / dot(ray.direction, this->normal));
        if(tplane < 0)
            return false;
        Hit hit1;
	Hit hit2;
        hit1.t = tplane;
        hit1.object = this;
        hit1.ray_exiting = false;
        hits.push_back(hit1);
        hit2.t = 0;
        hit2.object = this;
        if(dot(ray.direction, this->normal) < 0) 
            hit2.ray_exiting = false;
        else
            hit2.ray_exiting = true;
        hits.push_back(hit2);
        return true;
    }
    return false;
}

vec3 Plane::
Normal(const vec3& point) const
{
    return normal;
}
