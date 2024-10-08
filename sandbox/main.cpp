#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <stacked/ui.h>
#include <stacked/glfw_platform_data.h>

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Error: Failed to initialise GLAD.\n";
        return -1;
    }

    Ui::Initialise();
    Ui::InitialiseForGLFW(window);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        Ui::BeginFrameForGLFW();
        Ui::BeginFrame();

        Ui::BeginWindow("Window", {300, 200}, {0, 0});

        if (Ui::Button("Test", {100, 50}, {30, 10}))
        {
            std::cout << "Test button pressed.\n";
        }

        static float current = 0.0f;
        Ui::SliderFloat("Slider", current, 0.0f, 100.0f, {30, 80});

        static bool enabled = false;
        Ui::Checkbox("Checkbox", enabled, {30, 130});

        Ui::EndWindow();

        // Second window
        Ui::BeginWindow("Window2", {300, 200}, {300, 200});

        static std::string text;
        Ui::TextBox("Text Box 1", text, {200, 150}, {50, 25});

        Ui::EndWindow();

        Ui::EndFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Ui::Dispose();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}