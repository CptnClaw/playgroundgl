#include <iostream>
#include <memory>
#include "callbacks.h"
#include "shaders.h"
#include "Zm.h"

#define PI 3.14159f

void error_callback(int err, const char *msg)
{
    std::cout << "Error " << err << ": " << msg << std::endl;
}

void modify_by_action(int action, int value, int &to_modify)
{
    if (action == GLFW_PRESS)   to_modify = value;
    if (action == GLFW_RELEASE) to_modify = 0;
    // else do nothing
}

bool is_flashlight = false;
bool is_sun = true;
int move_x = 0, move_y = 0;
float rot_speed = 0.f;
float delta = .01f;
float light_strength = 1.f;

bool mouse_entered_focus = true;
double last_mouse_x, last_mouse_y;
float camera_pitch = 0.f, camera_yaw = PI / 2.f;
float flashlight_pitch = 0.f, flashlight_yaw = -PI / 2.f;
float mouse_sensitivity = .008f;
float zoom = 1;
float scroll_sensitivity = 0.1f;

bool mode_selection = false;
bool mouse_clicked = false;
uint click_x = 0;
uint click_y = 0;

// From main.cpp
extern Zm render_mode;
extern std::unique_ptr<Zm> cur_skybox;

void key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, [[maybe_unused]] int mods)
{
    int cursor_mode; // used only in case GLFW_KEY_4
    switch (key)
    {
    case GLFW_KEY_Q:
    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_CAPS_LOCK:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    case GLFW_KEY_1:
        // Cycle through render modes
        if (action == GLFW_PRESS)
        {
            render_mode.inc();
        }
        break;
    case GLFW_KEY_2:
        // Toggle flashlight
        if (action == GLFW_PRESS)
        {
            is_flashlight = !is_flashlight;
            flashlight_pitch = 0.f;
            flashlight_yaw = -PI / 2.f;
        }
        break;
    case GLFW_KEY_3:
        // Toggle sun
        if (action == GLFW_PRESS)
        {
            is_sun = !is_sun;
        }
        break;
    case GLFW_KEY_4:
        // Toggle object selection mode
        if (action == GLFW_PRESS)
        {
            mode_selection = !mode_selection;
        }
        cursor_mode = mode_selection ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
        glfwSetInputMode(window, GLFW_CURSOR, cursor_mode);
        break;
    case GLFW_KEY_W:
        modify_by_action(action, 1, move_y);
        break;
    case GLFW_KEY_A:
        modify_by_action(action, -1, move_x);
        break;
    case GLFW_KEY_S:
        modify_by_action(action, -1, move_y);
        break;
    case GLFW_KEY_D:
        modify_by_action(action, 1, move_x);
        break;
    case GLFW_KEY_EQUAL:
        rot_speed += delta;
        break;
    case GLFW_KEY_MINUS:
        rot_speed -= delta;
        break;
    case GLFW_KEY_K:
        light_strength += delta;
        if (light_strength > 1.0f) light_strength = 1.0f;
        break;
    case GLFW_KEY_J:
        light_strength -= delta;
        if (light_strength < 0.0f) light_strength = 0.0f;
        break;
    case GLFW_KEY_5:
        if (action == GLFW_PRESS)
        {
            cur_skybox->inc();
        }
        break;
    default:
        break;
    }
}

void fb_sz_callback([[maybe_unused]] GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void mouse_callback([[maybe_unused]] GLFWwindow *window, double x, double y)
{
    // Choose if mouse movements are tied to camera or flashlight
    // (state machine)
    float *modifier_pitch = &camera_pitch;
    float *modifier_yaw = &camera_yaw;
    if (is_flashlight)
    {
        modifier_pitch = &flashlight_pitch;
        modifier_yaw = &flashlight_yaw;
    }

    // Update selected pitch and yaw values according to mouse movements
    // Except for when mouse just entered focus, to avoid sudden jumps
    // And also except for when in object selection mode
    if (!mouse_entered_focus && !mode_selection)
    {
        *modifier_pitch += (float)(last_mouse_y - y) * mouse_sensitivity;
        *modifier_yaw += (float)(x - last_mouse_x) * mouse_sensitivity;
    }
    
    // Remember current mouse coordinates
    mouse_entered_focus = false;
    last_mouse_x = x;
    last_mouse_y = y;
}

void mouse_click_callback(GLFWwindow *window, int button, int action, [[maybe_unused]] int mods)
{
    double x, y;
    if (mode_selection && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &x, &y);
        mouse_clicked = true;
        click_x = static_cast<uint>(x);
        click_y = static_cast<uint>(y);
    }
}

void scroll_callback([[maybe_unused]] GLFWwindow *window, [[maybe_unused]] double xoffset, double yoffset)
{
    zoom -= yoffset * scroll_sensitivity;
}

void focus_callback([[maybe_unused]] GLFWwindow *window, int focused)
{
    if (focused)
    {
        mouse_entered_focus = true;
    }
    else
    {
        mouse_entered_focus = false;
    }
}
