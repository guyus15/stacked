#include "stacked/glfw_platform_data.h"
#include "stacked/input.h"

#include "ui_internal.h"

#include <GLFW/glfw3.h>

struct UiGlfwPlatformData_OpenGL
{
    GLFWwindow *window;
    GLFWcursor *cursor;
    double time;
};

void UiGlfwPlatformCallback_OpenGL_Key(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    UiIO &io = Ui::GetIO();
    io.input.KeyCallbackUpdate(key, action);
}

void UiGlfwPlatformCallback_OpenGL_MouseButton(GLFWwindow *window, int button, int action, int mods)
{
    UiIO &io = Ui::GetIO();
    io.input.MouseButtonCallbackUpdate(button, action);
}

void UiGlfwPlatformCallback_OpenGL_MousePosition(GLFWwindow *window, double x, double y)
{
    UiIO &io = Ui::GetIO();
    io.input.MousePositionCallbackUpdate(x, y);
}

void Ui::InitialiseForGLFW(GLFWwindow *window)
{
    UiIO &io = Ui::GetIO();
    UiGlfwPlatformData_OpenGL *platform = new UiGlfwPlatformData_OpenGL();

    UiContext *context = GetContext();
    io.platform_data = platform;

    platform->window = window;
    platform->cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    platform->time = 0;

    glfwSetKeyCallback(window, &UiGlfwPlatformCallback_OpenGL_Key);
    glfwSetMouseButtonCallback(window, &UiGlfwPlatformCallback_OpenGL_MouseButton);
    glfwSetCursorPosCallback(window, &UiGlfwPlatformCallback_OpenGL_MousePosition);
}

void Ui::BeginFrameForGLFW()
{
    UiIO &io = Ui::GetIO();
    UiGlfwPlatformData_OpenGL *pd = GetGlfwPlatformData_OpenGL();

    UiInt width, height;
    glfwGetWindowSize(pd->window, &width, &height);
    io.display_size = {width, height};

    double current_time = glfwGetTime();
    pd->time = current_time;
}