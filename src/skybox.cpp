#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "skybox.h"

Skybox::Skybox(std::string textures_directory) : texture(textures_directory)
{
    float vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    size_t stride = 3; // number of columns above

    // Create buffers on GPU
    glGenBuffers(1, &vbuf);
    glGenBuffers(1, &ibuf);
    glGenVertexArrays(1, &array_obj);
 
    // Copy vertices to GPU
    glBindVertexArray(array_obj);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Set vertex attribute: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

Skybox::~Skybox()
{
    std::cout << "NOTE: deleting skybox, VAO " << array_obj << std::endl;
    glDeleteVertexArrays(1, &array_obj);
    glDeleteBuffers(1, &vbuf);
}

void Skybox::draw(const Shaders &program, const glm::mat4 &view_matrix, const glm::mat4 &proj_matrix) const
{
    // Set depth function so that anything on the far plane (z == 1.0 in NDC) will be drawn
    glDepthFunc(GL_LEQUAL);
    
    // Activate and bind cubemap texture
    program.use();
    texture.activate(0);
    program.uniform_int("cubemap", 0);

    // Send transformations
    glm::mat4 view_sans_translations = glm::mat4(glm::mat3(view_matrix)); // remove translation component
    program.uniform_mat4("vp", proj_matrix * view_sans_translations);

    // Bind mesh and issue draw call
    glBindVertexArray(array_obj);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Revert depth function to default
    glDepthFunc(GL_LESS);
}