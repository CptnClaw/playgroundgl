#include <iostream>
#include "flashlight.h"

#define PI 3.14159f
extern float flashlight_pitch, flashlight_yaw;
extern bool is_flashlight;
const glm::vec3 FLASHLIGHT_COLOR(1.f, 1.f, 1.f);
const float FLASHLIGHT_STR = .9f;

Flashlight::Flashlight(float min_pitch, float max_pitch, float min_yaw, float max_yaw) : 
    min_pitch(min_pitch), max_pitch(max_pitch), min_yaw(min_yaw), max_yaw(max_yaw)
{
    // Left empty intentionally
}

glm::vec3 Flashlight::get_direction() const
{
    // Clamp flashlight angles 
    if (flashlight_pitch > max_pitch) flashlight_pitch = max_pitch;
    else if (flashlight_pitch < min_pitch) flashlight_pitch = min_pitch;
    if (flashlight_yaw > max_yaw) flashlight_yaw = max_yaw;
    else if (flashlight_yaw < min_yaw) flashlight_yaw = min_yaw;

    // Calculate direction based on given angles (determined in callbacks.cpp)
    glm::vec3 direc;
    direc.x = cos(flashlight_yaw) * cos(flashlight_pitch);
    direc.y = sin(flashlight_pitch);
    direc.z = sin(flashlight_yaw) * cos(flashlight_pitch);
    return direc;
}

void Flashlight::use(const Shaders &program, const glm::mat4 &view) const
{
    float is_on = 0.f;
    if (is_flashlight) is_on = 1.f;
    glm::vec3 world_direction = glm::vec3(glm::inverse(view) * glm::vec4(get_direction(), 1.f));
    program.uniform_vec3("flashlight.direction", world_direction);
    program.uniform_float("flashlight.is_on", is_on);
    program.uniform_vec3("flashlight.color", FLASHLIGHT_COLOR);
    program.uniform_float("flashlight.strength", FLASHLIGHT_STR);
}
