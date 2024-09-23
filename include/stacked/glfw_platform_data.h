#ifndef GLFW_PLATFORM_DATA
#define GLFW_PLATFORM_DATA

struct GLFWwindow;

namespace Ui
{
    void InitialiseForGLFW(GLFWwindow *window);
    void BeginFrameForGLFW();
}

#endif // GLFW_PLATFORM_DATA