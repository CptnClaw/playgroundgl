#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include <glm/glm.hpp>
#include "shaders.h"

// Point light
class LightSource 
{
public:
    LightSource(glm::vec3 position, glm::vec3 color);
    ~LightSource();
    glm::vec3 get_color() const;
    glm::vec3 get_position(const glm::mat4 &view) const; // Gets position in view space
    void update(float delta_time);
    void use(const Shaders &program, const glm::mat4 &view) const;
    void draw() const;

    glm::vec3 starting_position; // Position when constructing (at spin zero)
    glm::vec3 color; // Color of light
    float spin; // Speed of spinning around origin

    // World matrix parameter
    glm::mat4 model;

private:
    // OpenGL stuff
    uint vbuf; // Index of vertices buffer on GPU
    uint array_obj; // Index of array object on GPU
};

#endif // LIGHTSOURCE_H_