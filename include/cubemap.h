#ifndef CUBEMAP_H_ 
#define CUBEMAP_H_

#include <string>


// A specialized texture that can be sampled in all directions in 3D
class CubeMap
{
public:
    // Load cubemap from a set of image files in the following order: 
    // right, left, top, bottom, front, back
    CubeMap(std::string texture_directory);
    
    // Do not allow implicit copy due to OpenGL resource management
    CubeMap(const CubeMap&) = delete;
    CubeMap& operator=(const CubeMap&) = delete;

    // Free resources
    ~CubeMap();

    // Activate texture unit
    void activate(uint unit) const;

    // OpenGL index of texture
    uint id;
};


#endif // CUBEMAP_H_
