#include "lightsource.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.14159f
extern float light_strength;

LightSource::LightSource(glm::vec3 position, glm::vec3 color) :
    Box(position, 0.f), color(color), spin(0.5f)
{
    float scale = .05f;
    
    // Generate model matrix for starting position (before spinning)
    model = glm::mat4(1.f);
    model = glm::translate(model, starting_position);
    model = glm::scale(model, glm::vec3(scale));
}

glm::vec3 LightSource::get_color() const
{
    return color;
}

void LightSource::update(float delta_time)
{
    // Post-apply rotation matrix in order to spin around origin
    glm::mat4 rotation_matrix = glm::mat4(1.f); 
    float angle = delta_time * spin;
    rotation_matrix = glm::rotate(rotation_matrix, angle, glm::vec3(0.f, 0.f, 1.f));
    model = rotation_matrix * model;
}

glm::vec3 LightSource::get_position(const glm::mat4 &view) const
{
    return view * model * glm::vec4(starting_position, 1.f);
}

void LightSource::use(const Shaders &program, const glm::mat4 &view) const
{
    program.uniform_vec3("light.color", color);
    program.uniform_float("light.diffuse_intensity", 0.9);
    program.uniform_float("light.specular_intensity", 0.5);
    program.uniform_float("light.strength", light_strength);
    program.uniform_vec3("light.position", get_position(view));
}