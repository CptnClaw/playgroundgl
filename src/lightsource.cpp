#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lightsource.h"

#define PI 3.14159f
extern float light_strength;

glm::vec4 origin = glm::vec4(glm::vec3(0.f,0.f,0.f), 1.f);

LightSource::LightSource(glm::vec3 position, glm::vec3 color) : color(color), spin(1.f) 
{
    float vertices[] = {
        // 2D position  // (dummy) Normal  // (dummy) texture coords
        .0f, .0f, .0f,  0.f, 0.f, 0.f,     0.f, 0.f
    };
    size_t stride = 8; // number of columns above

    // Create buffers on GPU
    glGenBuffers(1, &vbuf);
    glGenVertexArrays(1, &array_obj);

    // Copy vertices to GPU
    glBindVertexArray(array_obj);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Set vertex attribute: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set vertex attribute: normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set vertex attribute: texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Generate model matrix for starting position (before spinning)
    model = glm::mat4(1.f);
    model = glm::translate(model, position);
}

LightSource::~LightSource()
{
    std::cout << "NOTE: deleting lightsource, VAO " << array_obj << std::endl;
    glDeleteVertexArrays(1, &array_obj);
    glDeleteBuffers(1, &vbuf);
}

void LightSource::update(float delta_time)
{
    // Post-apply rotation matrix in order to spin around origin
    glm::mat4 rotation_matrix = glm::mat4(1.f); 
    float angle = delta_time * spin;
    rotation_matrix = glm::rotate(rotation_matrix, angle, glm::vec3(0.f, 1.f, 0.f));
    model = rotation_matrix * model;
}

void LightSource::use(const Shaders &program) const
{
    program.uniform_vec3("light.color", color);
    program.uniform_float("light.diffuse_intensity", 0.9);
    program.uniform_float("light.specular_intensity", 0.9);
    program.uniform_float("light.strength", light_strength);
    program.uniform_vec3("light.position", model * origin);
}

void LightSource::draw() const
{
    glBindVertexArray(array_obj);
    glDrawArrays(GL_POINTS, 0, 1);
}