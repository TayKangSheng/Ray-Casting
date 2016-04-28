#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include "vecmath.h"

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"

using namespace std;

///TODO:
///Implement Shade function that uses ambient, diffuse, specular and texture
class Material
{
public:
	
    Material( const Vector3f& d_color ,const Vector3f& s_color=Vector3f::ZERO, float s=0): diffuseColor( d_color),specularColor(s_color), shininess(s){}

    virtual ~Material() {}

    virtual Vector3f getDiffuseColor() const
    {
        return  diffuseColor;
    }
    

    Vector3f Shade( const Ray& ray, const Hit& hit,
                   const Vector3f& dirToLight, const Vector3f& lightColor ) {
        
        Vector3f diffuse;
        Vector3f specular;
        
        
        float clampDiffuse = clampDotProduct(dirToLight, hit.getNormal() );
        
        if (t.valid()){
            Vector3f texColor = t(hit.texCoord[0], hit.texCoord[1]);
            diffuse = clampDiffuse*texColor*lightColor;
            diffuseColor = texColor;
        }
        else{
            diffuse = clampDiffuse*diffuseColor*lightColor;
        }
        
        
        float clampSpecular = clampDotProduct(dirToLight, (dirToLight*-1.0f) + 2.0f*hit.getNormal()*(clampDiffuse));
        specular = pow(clampSpecular, shininess) *lightColor*specularColor;
        
        return diffuse + specular;
        
    }

    void loadTexture(const char * filename){
        t.load(filename);
    }
    
    float clampDotProduct( Vector3f vec1, Vector3f vec2 ){
        float result = Vector3f::dot(vec1, vec2);
        
        if (result<0){
            result = 0;
        }

        return result;
    }
protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
    Texture t;
};



#endif // MATERIAL_H
