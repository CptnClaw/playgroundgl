#ifndef MODEL_H_
#define MODEL_H_

#include <memory>
#include <string>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include "mesh.h"

class Model
{
public:

    // Read a 3D model file and store it in GPU memory, read to draw
    Model(const std::string &filepath);

    // Draw call for every mesh in the model while activating their textures
    void draw(const Shaders &program, const glm::mat4 &view, const glm::mat4 &projection) const;

private:
    // Holds OpenGL resources for meshes and textures
    // in a unique_ptr mostly to avoid copies and double frees
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<std::unique_ptr<Texture>> texture_pool;

    // The "Model" matrix in "Model-View-Projection" transform
    glm::mat4 world_transform;
    
    // Traverse the model file while populating this object
    void processNode(aiNode *node, const aiScene *scene);
    void processMesh(aiMesh *mesh, const aiScene *scene);

    // Add new texture to pool lazily,
    // i.e. if it was already loaded previously, do nothing.
    // Returns texture id.
    uint lazy_add_to_pool(const std::string &texture_path, TextureType type);
};

#endif // MODEL_H_