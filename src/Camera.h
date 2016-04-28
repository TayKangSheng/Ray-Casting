#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "vecmath.h"
#include <float.h>
#include <cmath>


class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const Vector2f& point ) = 0 ;
	virtual float getTMin() const = 0 ; 
	virtual ~Camera(){}
    
protected:
	Vector3f center; 
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;

};

///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction,const Vector3f& up , float angle){
        // e (center)
        // u, v, w (horizontal, up, direction)
        
        
        this->center = center;
        this->w = direction.normalized();
        this->u = (Vector3f::cross(w, up)).normalized();
        this->v = (Vector3f::cross(this->u, this->w)).normalized();
        this->angle = angle;
        this->D = 1.0f/tan(this->angle/2.0f);
        
	}

	virtual Ray generateRay( const Vector2f& point){
//		return Ray(Vector3f( 1,0,0),Vector3f(1,0,0));
        float x = point[0];
        float y = point[1];
        
        //r = (x*u, aspect*y*v, D*w)
        Vector3f rayDir = (((this->u*x + this->v*y)) + this->D*this->w).normalized();
        
        return Ray(this->center, rayDir);
	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:
    Vector3f w,u,v;
    float D;
    float angle;
    float aspect = 1.0f;
};

#endif //CAMERA_H
