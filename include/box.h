#ifndef BOX_H_
#define BOX_H_

#include <glm/glm.hpp>

class Box
{
public:
    // Create a box at an initial position
    Box(glm::vec3 position, float angle);

    // Free resources
    ~Box();

    // Update box model matrix
    virtual void update(float delta_time);

    // Returns model matrix (world space)
    glm::mat4 get_model() const;

    // Draw box
    void draw() const;
    
    // Returns box position
    glm::vec3 get_position() const;

protected:
    // World matrix parameter
    glm::vec3 starting_position;
    float starting_angle;
    glm::mat4 model;

private:
    // OpenGL stuff
    uint vbuf; // Index of vertices buffer on GPU
    uint ibuf; // Index of indices buffer on GPU
    uint array_obj; // Index of array object on GPU
    int num_vertices;
};

#endif // BOX_H_