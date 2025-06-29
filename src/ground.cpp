#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ground.h"

Ground::Ground(float height, float scale, std::unique_ptr<Texture> diffuse, std::unique_ptr<Texture> specular) 
                : diffuse(std::move(diffuse)), specular(std::move(specular))
{
    // Define triangles
    float vertices[] = {
        // 2D position      // Normal        // texture coords
        -1.f, 0.f, -1.f,  0.f, 1.f, 0.f,    0.f, 0.f,      // bottom left
         1.f, 0.f, -1.f,  0.f, 1.f, 0.f,    scale, 0.f,      // bottom right
        -1.f, 0.f,  1.f,  0.f, 1.f, 0.f,    0.f, scale,      // top left
         1.f, 0.f,  1.f,  0.f, 1.f, 0.f,    scale, scale,      // top right
    };
    size_t stride = 8; // number of columns above
    uint indices[] = {
        0, 1, 2, 1, 2, 3, // back
    };

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

    // Set vertex attribute: normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set vertex attribute: texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Start with the identity matrix
    // Set starting position in world space
    model = glm::mat4(1.f); 
    model = glm::translate(model, glm::vec3(0.f, height, 0.f));
    model = glm::scale(model, glm::vec3(scale));
}

Ground::~Ground()
{
    std::cout << "NOTE: deleting ground, VAO " << array_obj << std::endl;
    glDeleteVertexArrays(1, &array_obj);
    glDeleteBuffers(1, &ibuf);
    glDeleteBuffers(1, &vbuf);
}

void Ground::draw(const Shaders &program, const glm::mat4 &view, const glm::mat4 &projection) const
{
    // Set up transform matrices
    program.use();
    program.set_transforms(model, view, projection);

    // Activate and bind textures
    diffuse->activate(0);
    program.uniform_int("material.diffuse_map1", 0);
    specular->activate(1);
    program.uniform_int("material.specular_map1", 1);
    program.uniform_float("material.shininess", 0.01f);

    // Bind mesh and issue draw call
    glBindVertexArray(array_obj);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

