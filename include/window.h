#ifndef WINDOW_H_
#define WINDOW_H_
#include <GLFW/glfw3.h>

class Window
{
    public:
        // Create window with OpenGL context
        // Make sure to check last argument to verify there were no errors
        Window(uint width, uint height, bool &success);

        // Free resources
        ~Window();

        // Prepares for next frame drawing by:
        // 1. Swaps buffers
        // 2. Polls events
        // 3. Clears screen
        // Returns true if the window is still open, false if should get ready to close
        bool next_frame_ready() const;

        bool is_initialized;
        GLFWwindow *handle;
};

#endif // WINDOW_H_