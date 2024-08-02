#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "ui.h"
#include "font.h"
#include "shader.h"
#include "rect.h"
#include "input.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

int main() {

    if (!glfwInit()) {
        std::cerr << "Error: Failed to initialise GLFW.\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow *window = glfwCreateWindow(800, 600, "UI Sandbox", nullptr, nullptr);

    if (!window) {
        std::cerr << "Error: Failed to create GLFW window.\n";
        return -1;
    }

    glfwMakeContextCurrent(window);

    Input::Initialise();

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, MousePositionCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error: Failed to initialise GLAD.\n";
        return -1;
    }

    std::string vertex_shader_path = "shaders/shader.vs";
    std::string fragment_shader_path = "shaders/shader.fs";

    std::string vertex_code, fragment_code;
    std::ifstream vertex_shader_file, fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertex_shader_file.open(vertex_shader_path);
        fragment_shader_file.open(fragment_shader_path);

        std::stringstream vertex_shader_stream, fragment_shader_stream;

        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        vertex_code = vertex_shader_stream.str();
        fragment_code = fragment_shader_stream.str();
    }
    catch (std::ifstream::failure&)
    {
        std::cerr << "Failed to read shader file.\n";
    }

    Shader shader{};
    shader.Compile(vertex_code, fragment_code);

    shader.Use();

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    shader.SetMat4("projection", projection);

    Rect rect{ 10, 10, 100, 100 };
    rect.SetColour(0.0f, 1.0f, 0.0f, 1.0f);
    rect.SetRadius(20.0f);

    while (!glfwWindowShouldClose(window)) 
    {
        glClear(GL_COLOR_BUFFER_BIT);

        int width = abs(cos(glfwGetTime()) * 750);

        if (Ui::Button("Balls", shader, { width, 200 }, { 25, 200 }))
        {
            std::cout << "Ow! That hurt!\n";
        }

        // if (Ui::Button("Don't Click Me", shader, { 200, 100 }, { 10, 10 }))
        // {
        //     std::cout << "I told you not to!\n";
        //     return -1;
        // }

        Input::Update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
