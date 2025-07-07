#ifndef GROUND_H_
#define GROUND_H_

#include <memory>
#include <glm/glm.hpp>
#include "texture.h"
#include "shaders.h"

class Ground
{
public:
    // Create a ground mesh at an initial height (z axis value) and textures
    Ground(float height, float scale, std::unique_ptr<Texture> diffuse, std::unique_ptr<Texture> specular);

    // Free resources
    virtual ~Ground();

    // Draw ground
    void draw(const Shaders &program) const;

    // World matrix parameter
    glm::mat4 model_transform;

private:
    
    // Ground textures
    std::unique_ptr<Texture> diffuse, specular;

    // OpenGL stuff
    uint vbuf; // Index of vertices buffer on GPU
    uint ibuf; // Index of indices buffer on GPU
    uint array_obj; // Index of array object on GPU
};

#endif // GROUND_H_
