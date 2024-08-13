/**
 * input.cpp
 */

#include "stacked/input.h"

#include <GLFW/glfw3.h>

enum class InputStatus
{
    NotPressed = 0,
    Pressed = 1
};

Input Input::s_instance{};

void Input::Initialise()
{
    for (int i = 0; i < static_cast<int>(KeyCode::KeyCodeSize); ++i)
    {
        Get().m_key_input[static_cast<KeyCode>(i)] = InputStatus::NotPressed;
        Get().m_key_input_prev[static_cast<KeyCode>(i)] = InputStatus::NotPressed;
    }
}

void Input::Update()
{
    Get().m_key_input_prev = Get().m_key_input;
    Get().m_mouse_input_prev = Get().m_mouse_input;
}

bool Input::GetKey(const KeyCode key)
{
    return Get().m_key_input[key] == InputStatus::Pressed;
}

bool Input::GetKeyDown(const KeyCode key)
{
    return Get().m_key_input[key] == InputStatus::Pressed &&
           Get().m_key_input_prev[key] == InputStatus::NotPressed;
}

bool Input::GetKeyUp(const KeyCode key)
{
    return Get().m_key_input[key] == InputStatus::NotPressed &&
           Get().m_key_input_prev[key] == InputStatus::Pressed;
}

bool Input::GetKeyHeld(const KeyCode key)
{
    return Get().m_key_input[key] == InputStatus::Pressed &&
           Get().m_key_input_prev[key] == InputStatus::Pressed;
}

bool Input::GetMouse(const MouseButton button)
{
    return Get().m_mouse_input[button] == InputStatus::Pressed;
}

bool Input::GetMouseDown(const MouseButton button)
{
    return Get().m_mouse_input[button] == InputStatus::Pressed &&
           Get().m_mouse_input_prev[button] == InputStatus::NotPressed;
}

bool Input::GetMouseUp(const MouseButton button)
{
    return Get().m_mouse_input[button] == InputStatus::NotPressed &&
           Get().m_mouse_input_prev[button] == InputStatus::Pressed;
}

bool Input::GetMouseHeld(const MouseButton button)
{
    return Get().m_mouse_input[button] == InputStatus::Pressed &&
           Get().m_mouse_input_prev[button] == InputStatus::Pressed;
}

UiVec2I Input::GetMousePosition()
{
    return Get().m_mouse_position;
}

Input &Input::Get()
{
    return s_instance;
}

void Input::KeyCallbackUpdate(const int glfw_keycode, const int glfw_action)
{
    const auto keycode = static_cast<KeyCode>(glfw_keycode);
    const auto input_status = glfw_action == GLFW_REPEAT ? InputStatus::Pressed : static_cast<InputStatus>(glfw_action);

    Get().m_key_input[keycode] = input_status;
}

void Input::MouseButtonCallbackUpdate(const int glfw_button, const int glfw_action)
{
    const auto button = static_cast<MouseButton>(glfw_button);
    const auto input_status = glfw_action == GLFW_REPEAT ? InputStatus::Pressed : static_cast<InputStatus>(glfw_action);

    Get().m_mouse_input[button] = input_status;
}

void Input::MousePositionCallbackUpdate(const double glfw_xpos, const double glfw_ypos)
{
    Get().m_mouse_position = {static_cast<int>(glfw_xpos), static_cast<int>(glfw_ypos)};
}

void KeyCallback(GLFWwindow *, const int keycode, int, const int action, int)
{
    Input::KeyCallbackUpdate(keycode, action);
}

void MouseButtonCallback(GLFWwindow *, const int button, const int action, int)
{
    Input::MouseButtonCallbackUpdate(button, action);
}

void MousePositionCallback(GLFWwindow *, const double xpos, const double ypos)
{
    Input::MousePositionCallbackUpdate(xpos, ypos);
}