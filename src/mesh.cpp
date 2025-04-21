#include <iostream>
#include <cstddef> // for offsetof
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, 
            const std::vector<uint> &indices,
            const std::vector<TextureHandle> &textures) : 
    num_indices(indices.size()), textures(textures)
{
    // Create buffers on GPU
    glGenBuffers(1, &vbuf);
    glGenBuffers(1, &ibuf);
    glGenVertexArrays(1, &array_obj);

    // Copy vertices to GPU
    glBindVertexArray(array_obj);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint), indices.data(), GL_STATIC_DRAW);
    
    // Set vertex attribute: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Set vertex attribute: normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    // Set vertex attribute: texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texture_coord)));
    glEnableVertexAttribArray(2);
}

Mesh::~Mesh()
{
    std::cout << "NOTE: deleting mesh, VAO " << array_obj << std::endl;
    glDeleteVertexArrays(1, &array_obj);
    glDeleteBuffers(1, &ibuf);
    glDeleteBuffers(1, &vbuf);
}

void Mesh::draw(const Shaders &program) const
{
    // Activate and bind textures
    int diffuse_index = 0;
    int specular_index = 0;
    for (const TextureHandle &t : textures)
    {
        int unit_id = diffuse_index + specular_index + 5;
        glActiveTexture(GL_TEXTURE0 + unit_id);
        glBindTexture(GL_TEXTURE_2D, t.id);
        if (t.type == TextureType::Diffuse)
        {
            program.uniform_int("material.diffuse_map" + std::to_string(++diffuse_index), unit_id);
        }
        else
        {
            program.uniform_int("material.specular_map" + std::to_string(++specular_index), unit_id);
        }
    }
    program.uniform_float("material.shininess", .5f);

    // Bind mesh and issue draw call
    glBindVertexArray(array_obj);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
}