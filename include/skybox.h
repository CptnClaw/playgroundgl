#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <string>
#include "shaders.h"
#include "cubemap.h"

class Skybox
{
public:
    // Initialize a textured skybox. Texture files must be inside the directory with the following filenames:
    // right.jpg, left.jpg, top.jpg, bottom.jpg, front.jpg, back.jpg 
    Skybox(std::string textures_directory);

    // Free resources
    ~Skybox();

    // Do not allow implicit copy due to OpenGL resource management
    Skybox(const Skybox&) = delete;
    Skybox& operator=(const Skybox&) = delete;
    
    // Issue draw call
    void draw(const Shaders &program, const glm::mat4 &view_matrix, const glm::mat4 &proj_matrix) const;

private:
    // OpenGL stuff
    uint vbuf; // Index of vertices buffer on GPU
    uint array_obj; // Index of array object on GPU

    CubeMap texture;
};

#endif  // SKYBOX_H_
