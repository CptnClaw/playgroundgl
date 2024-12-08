#ifndef FLASHLIGHT_H_
#define FLASHLIGHT_H_

#include <glm/glm.hpp>

class Flashlight
{
public:
    // Construct flashlight
    Flashlight(float min_pitch, float max_pitch, float min_yaw, float max_yaw);
    
    // Calculate direction based on flashlight_pitch and flashlight_yaw (extern vars from callback.cpp)
    glm::vec4 get_direction();

private:
    float min_pitch, max_pitch;
    float min_yaw, max_yaw;
};


#endif // FLASHLIGHT_H_