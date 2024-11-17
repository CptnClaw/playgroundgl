#ifndef BOX_H_
#define BOX_H_

#include <glm/glm.hpp>

class Box
{
public:
    // Create a box at an initial position
    Box(glm::vec3 position, float rotation, float scale, bool keep_rotating);

    // Free resources
    ~Box();

    // Update box model matrix
    void update(float delta_time);

    // Returns model matrix (world space)
    glm::mat4 get_model() const;

    // Draw box
    void draw() const;

private:
    // World matrix parameter
    glm::vec3 position;
    float rotation, scale;
    bool keep_rotating;
    glm::mat4 model;

    // OpenGL stuff
    uint vbuf; // Index of vertices buffer on GPU
    uint ibuf; // Index of indices buffer on GPU
    uint array_obj; // Index of array object on GPU
    int num_vertices;
};

#endif // BOX_H_