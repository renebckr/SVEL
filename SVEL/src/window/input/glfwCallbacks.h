#ifndef EVENTS_GLFW_CALLBACKS_H
#define EVENTS_GLFW_CALLBACKS_H

#include <functional>
#include <GLFW/glfw3.h>

namespace GlfwCallback
{
    using KeyFunc       = std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)>;
    using CursorPosFunc = std::function<void(GLFWwindow* window, double xpos, double ypos)>;

    struct Callbacks
    {
        KeyFunc KeyHandler;
        CursorPosFunc CursorPosHandler;
    };

    void SetKeyFunc(GLFWwindow* _window, KeyFunc _callback);
    void SetCursorPosFunc(GLFWwindow* _window, CursorPosFunc _callback);
}

#endif /* EVENTS_GLFW_CALLBACKS_H */