#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <stacked/input.h>
#include <stacked/ui.h>

#include <iostream>
#include <cstdlib>

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Error: Failed to initialise GLFW.\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow *window = glfwCreateWindow(800, 600, "UI Sandbox", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Error: Failed to create GLFW window.\n";
        return -1;
    }

    glfwMakeContextCurrent(window);

    Input::Initialise();

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, MousePositionCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Error: Failed to initialise GLAD.\n";
        return -1;
    }

    Ui::Initialise();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        Ui::BeginFrame();

        Ui::BeginWindow("Window", {600, 400}, {100, 100});

        if (Ui::Button("Test", {100, 50}, {30, 30}))
        {
            std::cout << "Test button pressed.\n";
        }

        static float current = 0.0f;
        Ui::SliderFloat("Slider", current, 0.0f, 100.0f, {30, 100});

        static bool enabled = false;
        Ui::Checkbox("Checkbox", enabled, {30, 150});

        Ui::EndWindow();

        Ui::EndFrame();

        Input::Update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Ui::Dispose();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}