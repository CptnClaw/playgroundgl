#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

#define PI 3.14159f
#define EPSILON .001f
#define ALMOST_PI (PI-EPSILON)
#define MOVE_SPEED 1.f

extern int move_x, move_y;
extern float pitch, yaw;
extern float zoom;

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 world_up) : position(position), direction(direction), world_up(world_up)
{
    // Left empty intentionally
}

glm::mat4 Camera::create_view(float delta_time)
{
    if (pitch > ALMOST_PI / 2) pitch = ALMOST_PI / 2;
    else if (pitch < -(ALMOST_PI) / 2) pitch = -(ALMOST_PI) / 2;
    direction.x = cos(yaw) * cos(pitch);
    direction.y = sin(pitch);
    direction.z = sin(yaw) * cos(pitch);
    direction = glm::normalize(direction);
    glm::vec3 camera_right = glm::normalize(glm::cross(direction, world_up));
    position = position + 
                direction * (MOVE_SPEED * delta_time * move_y) + 
                camera_right * (MOVE_SPEED * delta_time * move_x);
    return glm::lookAt(position, position + direction, world_up);
}

glm::mat4 Camera::create_projection(float aspect_ratio) const
{
    if (zoom > 1.f) zoom = 1.f;
    if (zoom < .2f) zoom = .2f;
    float fov = zoom * (PI / 4);
    return glm::perspective(fov, aspect_ratio, .1f, 100.f);
}