#include "bounded_plane.h"
#include "ray.h"
#include <cfloat>
#include <complex>
#include <limits>
#include <vector>


// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.

bool BoundedPlane::
Intersection(const Ray& ray, std::vector<Hit>& hits) const 
{
    std::vector<Hit> mhits; 
    bool intersects=Plane::Intersection(ray,mhits);
    if(!intersects) return false;
    for(unsigned int i=0;i<mhits.size();i++)
        if(Is_Inside_Bounding_Box(ray.Point(mhits[i].t)))
            hits.push_back(mhits[i]);
    return !hits.empty();
}

vec3 BoundedPlane::
Normal(const vec3& point) const 
{
  return normal;
}
