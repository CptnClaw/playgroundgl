#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>

class Camera
{
public:
    // Construct camera
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 world_up);
    
    // Generate view and perspective projection matrices
    glm::mat4 create_view(float delta_time);
    glm::mat4 create_projection(float aspect_ratio) const;

private:
    glm::vec3 position, direction, world_up;
};

#endif // CAMERA_H_