#ifndef __BOUNDED_PLANE_H__
#define __BOUNDED_PLANE_H__

#include "plane.h"

class BoundedPlane : public Plane
{
public:
    vec3 min_corner;
    vec3 max_corner;

    BoundedPlane(const vec3& point,const vec3& normal,const vec3& min_corner,const vec3& max_corner)
        :Plane(point,normal),min_corner(min_corner),max_corner(max_corner)
    {}

    virtual bool Intersection(const Ray& ray, std::vector<Hit>& hits) const override;

    virtual vec3 Normal(const vec3& point) const override;

    bool Is_Inside_Bounding_Box(const vec3& x) const
    { return (x[0]>=min_corner[0] && x[1]>=min_corner[1] && x[2]>=min_corner[2] && 
            x[0]<=max_corner[0] && x[1]<=max_corner[1] && x[2]<=max_corner[2]);}


};
#endif
