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
    
    // Draw call for every mesh in the model without activating textures and only using mvp transformation matrix for very simple shaders
    void draw_simple(const Shaders &program, const glm::mat4 &mvp) const;
        
    // Draw using a stencil trick to show outline around model
    void draw_with_outline(const Shaders &program, const Shaders &outline, const glm::mat4 &view, const glm::mat4 &projection) const;

    // Update model matrix (world_transform)
    void translate(float x, float y, float z);
    void rotate(float angle, float axis_x, float axis_y, float axis_z);
    void scale(float amount);
    void spin(float delta_time);

    // Print a message about successful loading and a count of texture types
    void print_debug_stats(const std::string &filepath);

    // The "Model" matrix in "Model-View-Projection" transform
    glm::mat4 world_transform;

private:
    // Holds OpenGL resources for meshes and textures
    // in a unique_ptr mostly to avoid copies and double frees
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<std::unique_ptr<Texture>> texture_pool;
    
    // Traverse the model file while populating this object
    void processNode(aiNode *node, const aiScene *scene);
    void processMesh(aiMesh *mesh, const aiScene *scene);

    // Add new texture to pool lazily,
    // i.e. if it was already loaded previously, do nothing.
    // Returns texture id.
    uint lazy_add_to_pool(const std::string &texture_path, TextureType type);
    
    // The directory of the object files, where the textures should be located
    std::string directory;
};

#endif // MODEL_H_