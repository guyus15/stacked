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

        int width = 500;

        static float current = 100.0f;
        Ui::SliderFloat("Slider", current, 0.0f, 150.0f, {300, 500});

        static float current2 = 50.0f;
        Ui::SliderFloat("Slider2", current2, 0.0f, 100.0f, {300, 400});

        static bool enabled = false;
        Ui::Checkbox("Checkbox", enabled, {400, 100});

        if (Ui::Button(std::to_string(static_cast<int>(current)), {width, 200}, {25, 200}))
        {
            std::cout << "Ow! That hurt!\n";
        }

        if (enabled)
        {
            if (Ui::Button("Don't Click Me", {200, 100}, {10, 10}))
            {
                std::cout << "I told you not to!\n";
                return -1;
            }
        }

        Input::Update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
