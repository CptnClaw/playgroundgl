#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include "box.h"

class LightSource : public Box
{
public:
    LightSource(glm::vec3 position, glm::vec3 color);
    glm::vec3 get_color() const;
    glm::vec4 get_position(glm::mat4 view) const; // Gets position in view space
    virtual void update(float delta_time);

private:
    glm::vec3 color; // Color of light
    float spin; // Speed of spinning around origin
};

#endif // LIGHTSOURCE_H_