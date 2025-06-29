#include <iostream>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

#define PI 3.14159f
#define EPSILON .001f
#define ALMOST_PI (PI-EPSILON)
#define MOVE_SPEED 6.f

extern int move_x, move_y;
extern float camera_pitch, camera_yaw;
extern float zoom;

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 world_up, float aspect_ratio) : 
            position(position), direction(direction), world_up(world_up), aspect_ratio(aspect_ratio)
{
    // Left empty intentionally
}

void Camera::update(float delta_time)
{
    // Clamp camera pitch to [-pi/2, pi/2].
    if (camera_pitch > ALMOST_PI / 2) camera_pitch = ALMOST_PI / 2;
    else if (camera_pitch < -(ALMOST_PI) / 2) camera_pitch = -(ALMOST_PI) / 2;

    // Update view matrix
    direction.x = cos(camera_yaw) * cos(camera_pitch);
    direction.y = sin(camera_pitch);
    direction.z = sin(camera_yaw) * cos(camera_pitch);
    direction = glm::normalize(direction);
    glm::vec3 camera_right = glm::normalize(glm::cross(direction, world_up));
    position = position + 
                direction * (MOVE_SPEED * delta_time * move_y) + 
                camera_right * (MOVE_SPEED * delta_time * move_x);
    position.y = position.y > -.8f ? position.y : -.8f;
    glm::vec3 camera_target = position + direction;
    view = glm::lookAt(position, camera_target, world_up);
    
    // Update projection matrix
    if (zoom > 1.f) zoom = 1.f;
    if (zoom < .2f) zoom = .2f;
    float fov = zoom * (PI / 4);
    projection = glm::perspective(fov, aspect_ratio, .1f, 100.f);
}

glm::mat4 Camera::get_view() const
{
    return view;
}

glm::mat4 Camera::get_projection() const
{
    return projection;
}