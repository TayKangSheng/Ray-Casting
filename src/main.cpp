#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <string>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>
#include <stdlib.h>

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

Vector2f imagePixelToCameraPoint(int width, int height, int pixelX, int pixelY){
    float midX = (width*1.0f)/2;
    float midY = (height*1.0f)/2;
    
    float coorX = (pixelX-midX)/midX;
    float coorY = ((pixelY-midY)*(1.0f) )/midY;
    
    return Vector2f(coorX, coorY);
}



#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
    // Fill in your implementation here.
    char* filename = NULL;
    int size[2], depth[2];
    char* output = NULL;
    char* depthOutput = NULL;
    char* normalOutput = NULL;
    
    // This loop loops over each of the input arguments.
    // argNum is initialized to 1 because the first
    // "argument" provided to the program is actually the
    // name of the executable (in our case, "a4").
    for( int argNum = 1; argNum < argc; ++argNum )
    {
        
        cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
        
        if (strcmp(argv[argNum],"-input") == 0){
            filename = argv[argNum+1];
            
        }
        
        if (strcmp(argv[argNum],"-size") == 0){
            size[0] = atoi(argv[argNum+1]);
            size[1] = atoi(argv[argNum+2]);
            
        }
        
        if (strcmp(argv[argNum],"-output") == 0){
            output = argv[argNum+1];

        }
        
        if (strcmp(argv[argNum],"-depth") == 0){
            depth[0] = atoi(argv[argNum+1]);
            depth[1] = atoi(argv[argNum+2]);
            depthOutput = argv[argNum+3];
        }
        
        if (strcmp(argv[argNum],"-normals") == 0){
            normalOutput = argv[argNum+1];
        }
        
    }
    
    

    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    
    /*
     Write the main function that reads the scene (using the parsing code provided), 
     loops over the pixels in the image plane, generates a ray using your camera class, 
     intersects it with the high-level Group that stores the objects of the scene, and 
     writes the color of the closest intersected object. 
     
     Up to this point, you may choose to render some spheres with a single color. If there is an intersection, use one color and if not, use another color.
     */
    
    SceneParser sceneParser = SceneParser(filename);
    Group* group = sceneParser.getGroup();
    
    Image image = Image(size[0], size[1]);
    Camera* camera = sceneParser.getCamera();
    
    image.SetAllPixels(sceneParser.getBackgroundColor());

    for (int i=0 ; i<size[0] ; i++){
        for (int j=0 ; j<size[1] ; j++){
            Hit hit = Hit(FLT_MAX, NULL, Vector3f(0.0f,0.0f,0.0f));
            Ray ray = sceneParser.getCamera()->generateRay( imagePixelToCameraPoint(size[0], size[1], i, j) );
            group->intersect( ray, hit, camera->getTMin() );
            
            if (hit.getMaterial() != NULL){
                Vector3f c_pixel = Vector3f(0.0f, 0.0f, 0.0f);
                Vector3f c_ambient = sceneParser.getAmbientLight();
                
                c_pixel = c_pixel + (hit.getMaterial()->getDiffuseColor()*c_ambient);
                
                for (int l=0 ; l<sceneParser.getNumLights() ; l++){
                    Light* light = sceneParser.getLight(l);
                    Vector3f dir;
                    Vector3f col;
                    float distanceToLight;
                    
                    light->getIllumination(ray.pointAtParameter(hit.getT()), dir, col, distanceToLight);
                    
                    Vector3f shade = hit.getMaterial()->Shade(ray, hit, dir, col);
                    
                    c_pixel = c_pixel + shade;
                }
                
                image.SetPixel(i, j, c_pixel);
                
            }
        }
    }


    ///TODO: below demonstrates how to use the provided Image class
    ///Should be removed when you start

    image.SaveImage( output);
    return 0;
}










