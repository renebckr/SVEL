#include "glfwCallbacks.h"
#include <GLFW/glfw3.h>

namespace GlfwCallback
{
    GlfwCallback::Callbacks g_callbacks;
}

void DefaultKeyFunc(GLFWwindow* _window, int _a, int _b, int _c, int _d)
{
    GlfwCallback::g_callbacks.KeyHandler(_window, _a, _b, _c, _d);
}

void DefaultCursorPosFunc(GLFWwindow* _window, double _a, double _b)
{
    GlfwCallback::g_callbacks.CursorPosHandler(_window, _a, _b);
}

void GlfwCallback::SetKeyFunc(GLFWwindow* _window, KeyFunc _callback)
{
    GlfwCallback::g_callbacks.KeyHandler = _callback;
    glfwSetKeyCallback(_window, DefaultKeyFunc);
}

void GlfwCallback::SetCursorPosFunc(GLFWwindow* _window, CursorPosFunc _callback)
{
    GlfwCallback::g_callbacks.CursorPosHandler = _callback;
    glfwSetCursorPosCallback(_window, DefaultCursorPosFunc);
}
