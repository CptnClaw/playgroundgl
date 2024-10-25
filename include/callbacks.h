#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <glad/gl.h>
#include <GLFW/glfw3.h>

void error_callback(int err, const char *msg);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void fb_sz_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double x, double y);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);


#endif // CALLBACKS_H_

