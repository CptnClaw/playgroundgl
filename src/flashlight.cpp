#include <iostream>
#include "flashlight.h"

extern float flashlight_pitch, flashlight_yaw;
extern bool is_flashlight;
#define PI 3.14159f

Flashlight::Flashlight(float min_pitch, float max_pitch, float min_yaw, float max_yaw) : 
    min_pitch(min_pitch), max_pitch(max_pitch), min_yaw(min_yaw), max_yaw(max_yaw)
{
    // Left empty intentionally
}

glm::vec4 Flashlight::get_direction()
{
    // Clamp flashlight angles 
    if (flashlight_pitch > max_pitch) flashlight_pitch = max_pitch;
    else if (flashlight_pitch < min_pitch) flashlight_pitch = min_pitch;
    if (flashlight_yaw > max_yaw) flashlight_yaw = max_yaw;
    else if (flashlight_yaw < min_yaw) flashlight_yaw = min_yaw;
    
    float turned_on = 0.f;
    if (is_flashlight) turned_on = 1.f;

    // Calculate direction based on given angles (determined in callbacks.cpp)
    glm::vec3 direc;
    direc.x = cos(flashlight_yaw) * cos(flashlight_pitch);
    direc.y = sin(flashlight_pitch);
    direc.z = sin(flashlight_yaw) * cos(flashlight_pitch);
    direc = glm::normalize(direc);
    return glm::vec4(direc, turned_on);
}