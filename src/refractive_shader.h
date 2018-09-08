#ifndef __REFRACTIVE_SHADER_H__
#define __REFRACTIVE_SHADER_H__

#include "shader.h"

namespace REFRACTIVE_INDICES{

    const double AIR=1.00;
    const double WATER=1.34;
    const double GLASS=1.70;
    const double DIAMOND=2.42;
}
class Refractive_Shader : public Shader
{
public:
    Shader* shader;
    double refractive_index;
    double opaqueness;
    
    Refractive_Shader(Render_World& world_input,Shader* shader_input,double refractive_index,double opaqueness)
        :Shader(world_input),shader(shader_input),
        refractive_index(std::max(0.0,refractive_index)),opaqueness(std::max(0.0,std::min(1.0,opaqueness)))
    {}

     virtual vec3 Shade_Surface(const Ray& ray,const vec3& intersection_point,
         const vec3& same_side_normal,int recursion_depth,bool is_exiting) const override;

     void Enforce_Refractance_Ratio(double& reflectance_ratio) const;
};
#endif
