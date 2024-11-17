#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>

class Camera
{
public:
    // Construct camera
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 world_up, float aspect_ratio);
    
    // Generate view and perspective projection matrices
    void update(float delta_time);
    glm::mat4 get_view() const;
    glm::mat4 get_projection() const;

private:
    glm::vec3 position, direction, world_up;
    float aspect_ratio;
    glm::mat4 view, projection;
};

#endif // CAMERA_H_