#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "box.h"

#define PI 3.14159f

extern float rot_speed;

Box::Box(glm::vec3 position, float rotation, float scale, bool keep_rotating) : 
        position(position), rotation(rotation), scale(scale), keep_rotating(keep_rotating)
{
    // Define triangles
    float vertices[] = {
        // 2D position    // texture coords
        -.5f, -.5f, -.5f,      0.f, 0.f,      // bottom left
         .5f, -.5f, -.5f,      1.f, 0.f,      // bottom right
        -.5f,  .5f, -.5f,      0.f, 1.f,      // top left
         .5f,  .5f, -.5f,      1.f, 1.f,      // top right
                                              //
        -.5f, -.5f, .5f,       0.f, 0.f,      // 
         .5f, -.5f, .5f,       1.f, 0.f,      // 
        -.5f,  .5f, .5f,       0.f, 1.f,      // 
         .5f,  .5f, .5f,       1.f, 1.f,      // 
                                              // 
        -.5f,  .5f, .5f,       0.f, 0.f,      // 
         .5f,  .5f, .5f,       1.f, 0.f,      // 
        -.5f,  .5f, -.5f,      0.f, 1.f,      // 
         .5f,  .5f, -.5f,      1.f, 1.f,      // 
                                              //
        -.5f, -.5f, .5f,       0.f, 0.f,      // 
         .5f, -.5f, .5f,       1.f, 0.f,      // 
        -.5f, -.5f, -.5f,      0.f, 1.f,      // 
         .5f, -.5f, -.5f,      1.f, 1.f,      // 
                                              //
         .5f,  .5f,  .5f,      0.f, 0.f,      // 
         .5f, -.5f,  .5f,      1.f, 0.f,      // 
         .5f,  .5f, -.5f,      0.f, 1.f,      // 
         .5f, -.5f, -.5f,      1.f, 1.f,      // 
                                              //
         -.5f,  .5f, .5f,      0.f, 0.f,      // 
         -.5f, -.5f, .5f,      1.f, 0.f,      // 
         -.5f,  .5f,-.5f,      0.f, 1.f,      // 
         -.5f, -.5f,-.5f,      1.f, 1.f       // 
    };
    size_t stride = 5; // number of columns above
    uint indices[] = {
        0, 1, 2, 1, 2, 3, // back
        4, 5, 6, 5, 6, 7, // front
        8, 9,10, 9,10,11, // top
        12,13,14,13,14,15,// bottom
        16,17,18,17,18,19,// right
        20,21,22,21,22,23 // left
    };
    num_vertices = sizeof(indices) / sizeof(uint);

    // Create buffers on GPU
    glGenBuffers(1, &vbuf);
    glGenBuffers(1, &ibuf);
    glGenVertexArrays(1, &array_obj);

    // Copy vertices to GPU
    glBindVertexArray(array_obj);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Set vertex attribute: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set vertex attribute: texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

Box::~Box()
{
    glDeleteVertexArrays(1, &array_obj);
    glDeleteBuffers(1, &ibuf);
    glDeleteBuffers(1, &vbuf);
}

void Box::update(float delta_time)
{
    // Start with the identity matrix
    model = glm::mat4(1.f); 
    
    // Set position in world space
    model = glm::translate(model, position); 
    model = glm::scale(model, glm::vec3(scale));
    
    // Set rotation
    if (keep_rotating)
    {
        rotation += delta_time * rot_speed;
    }
    model = glm::rotate(model,  rotation * -PI / 3, glm::vec3(1.f, 2.f, 0.f));
}

glm::mat4 Box::get_model() const
{
    return model;
}

void Box::draw() const
{
    glBindVertexArray(array_obj);
    glDrawElements(GL_TRIANGLES, num_vertices, GL_UNSIGNED_INT, 0);
}

