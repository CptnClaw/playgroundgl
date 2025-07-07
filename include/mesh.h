#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <glm/glm.hpp>
#include "texture.h"
#include "shaders.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coord;
};

struct TextureHandle
{
    uint id;
    TextureType type;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex> &vertices, 
        const std::vector<uint> &indices,
        const std::vector<TextureHandle> &textures); 

    // Do not allow implicit copy due to OpenGL resource management
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Free resources
    ~Mesh();

    // Issue draw call
    void draw(const Shaders &program, bool with_textures) const;

private:
    // OpenGL stuff
    uint vbuf; // Index of vertices buffer on GPU
    uint ibuf; // Index of indices buffer on GPU
    uint array_obj; // Index of array object on GPU

    // Mesh data
    size_t num_indices;
    std::vector<TextureHandle> textures;
};

#endif // MESH_H_