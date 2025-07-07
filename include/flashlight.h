#ifndef FLASHLIGHT_H_
#define FLASHLIGHT_H_

#include <glm/glm.hpp>
#include "shaders.h"

class Flashlight
{
public:
    // Construct flashlight
    Flashlight(float min_pitch, float max_pitch, float min_yaw, float max_yaw);
    void use(const Shaders &program, const glm::mat4 &view) const;

private:
    float min_pitch, max_pitch;
    float min_yaw, max_yaw;

    // Calculate direction based on flashlight_pitch and flashlight_yaw (extern vars from callback.cpp)
    glm::vec3 get_direction() const;
};


#endif // FLASHLIGHT_H_