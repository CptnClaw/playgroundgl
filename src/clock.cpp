#include <GLFW/glfw3.h>
#include "clock.h"

Clock::Clock() : prev_time(glfwGetTime())
{
    // Left empty intentionally
}

float Clock::tick()
{
    float cur_time = (float)glfwGetTime();
    float time_diff = cur_time - prev_time;
    prev_time = cur_time;
    return time_diff;
}