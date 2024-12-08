#include "sun.h"

extern bool is_sun;
const glm::vec3 SUN_COLOR(1.f, 1.f, 1.f);
const float SUN_STR = 1.f;

Sun::Sun(glm::vec3 direction) : direction(direction)
{
    // Left empty intentionally
}

void Sun::use(const Shaders &program, const glm::mat4 &view) const
{
    float is_on = 0.f;
    if (is_sun) is_on = 1.f;
    program.uniform_float("sun.diffuse_intensity", 0.9);
    program.uniform_float("sun.specular_intensity", 0.5);
    program.uniform_vec3("sun.direction", view * glm::vec4(direction, 0.f));
    program.uniform_float("sun.is_on", is_on);
    program.uniform_vec3("sun.color", SUN_COLOR);
    program.uniform_float("sun.strength", SUN_STR);
}