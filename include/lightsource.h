#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include <glm/glm.hpp>
#include "box.h"
#include "shaders.h"

class LightSource : public Box
{
public:
    LightSource(glm::vec3 position, glm::vec3 color);
    glm::vec3 get_color() const;
    glm::vec3 get_position(const glm::mat4 &view) const; // Gets position in view space
    virtual void update(float delta_time);
    void use(const Shaders &program, const glm::mat4 &view) const;

private:
    glm::vec3 color; // Color of light
    float spin; // Speed of spinning around origin
};

#endif // LIGHTSOURCE_H_