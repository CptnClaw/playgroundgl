#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include <glm/glm.hpp>
#include "shaders.h"

// Point light
class LightSource 
{
public:
    LightSource(glm::vec3 position, glm::vec3 color);

    // Do not allow implicit copy due to OpenGL resource management
    LightSource(const LightSource&) = delete;
    LightSource& operator=(const LightSource&) = delete;

    // Free OpenGL resources
    ~LightSource();

    // Spin around starting position
    void update(float delta_time);

    // Send transformations to shader
    void use(const Shaders &program) const;
    
    // Draw call
    void draw() const;

    // Members
    glm::vec3 color; // Color of light
    float spin; // Speed of spinning around origin
    glm::mat4 model; // World matrix

private:
    // OpenGL stuff
    uint vbuf; // Index of vertices buffer on GPU
    uint array_obj; // Index of array object on GPU
};

#endif // LIGHTSOURCE_H_