#include "boolean.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "hit.h"
#include "shader.h"
#include "render_world.h"


// Determine if the ray intersects with the boolean of A and B.
bool Boolean::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    std::vector<Hit> bool_set_A;
    std::vector<Hit> bool_set_B;

    bool bool_A = A->Intersection(ray, bool_set_A);
    bool bool_B = B->Intersection(ray, bool_set_B);


    if((type == type_union) && (bool_A || bool_B)){
	    //cout << "is_union" << endl;

	unsigned int maxi = std::max(bool_set_A.size(), bool_set_B.size());

	for(unsigned int i = 0; i < maxi; i++){
//cout << "union" << endl;
	    if(bool_set_B.size() == 0){
	        hits.push_back(bool_set_A[i]);
	        continue;
	    }else if(bool_set_A.size() == 0){
	        hits.push_back(bool_set_B[i]);
	        continue;
	    }

	    if(i > bool_set_A.size() && i < bool_set_B.size()){
	        //cout << "here" << endl;
	        hits.push_back(bool_set_B[i]);
	        continue;
	    }

	    if(i > bool_set_B.size() && i < bool_set_A.size()){
	        //cout << "here" << endl;
	        hits.push_back(bool_set_A[i]);
	        continue;
	    }
		
	    if(bool_set_A[i].t < bool_set_B[i].t){

	        hits.push_back(bool_set_A[i]);

	    }else{
	        hits.push_back(bool_set_B[i]);
	    }
	}

        return true;

    }else if(type == type_intersection && (bool_A && bool_B)){

	unsigned int maxi = std::max(bool_set_A.size(), bool_set_B.size());

	for(unsigned int i = 0; i < maxi; i++){
		
	    if(bool_set_A[i].t < bool_set_B[i].t){

	        hits.push_back(bool_set_A[i]);

	    }else{
	        hits.push_back(bool_set_B[i]);
	    }
	}

        return true;

    }else if(type == type_difference && (bool_A)){

	//cout << "difference" << endl;

        //cout << "boolA " << bool_set_A.size() << endl;
        //cout << "boolB " << bool_set_B.size() << endl;
	//unsigned int maxi = max(bool_set_A.size(), bool_set_B.size());

	bool good = true;

	if(bool_set_A.size() == 0){
	
	    return false;
	}	

	for(unsigned int i = 0; i < bool_set_A.size(); i++){
	    for(unsigned int j = 0; j < bool_set_B.size(); j++){
		if(bool_set_A[i].t != bool_set_B[j].t){
		    continue;
		}
		    
		good = false;
		
	
		//good = true;
	    }

	    if(good){
		//cout << "object A: " << bool_set_A[i].t << endl;
		//cout << bool_set_A.size() + bool_set_B.size() << endl;
		
		hits.push_back(bool_set_A[i]);
	    }
	    /*
	    if(bool_set_B.size() == 0){
	        hits.push_back(bool_set_A[i]);
	        continue;
	    }
	    if(i < bool_set_B.size() && bool_set_A[i].t != bool_set_B[i].t){
	        hits.push_back(bool_set_A[i]);
	    }*/

	    
	}

        return true;

    }else{
	return false;
    }
}

// This should never be called.
vec3 Boolean::Normal(const vec3& point) const
{
    assert(false);
    return vec3();
}
