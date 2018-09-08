#include "sphere.h"
#include "ray.h"


// Determine if the ray intersects with the sphere
bool Sphere::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    vec3 p = ray.endpoint - this->center;
    double D = (dot(p, ray.direction) * dot(p, ray.direction)) - p.magnitude_squared() + (this->radius * this->radius);
    if(D > 0) {
        double t1 = -dot(p, ray.direction) - sqrt(D);
        double t2 = -dot(p, ray.direction) + sqrt(D);
        if(t2 > 0) {
            Hit hit2;
            hit2.t = t2;
            hit2.object = this;
            hit2.ray_exiting = true;
            hits.push_back(hit2);
            if(t1 > 0) {
                Hit hit1;
                hit1.t = t1;
                hit1.object = this;
                hit1.ray_exiting = false;
                hits.push_back(hit1);
            }
	    return true;
        }
        
    }
    return false;
}

vec3 Sphere::Normal(const vec3& point) const
{
    vec3 normal;
    // TODO: set the normal
    normal = (point - center).normalized();
    return normal;
}
