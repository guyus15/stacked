/**
 * input.cpp
 */

#include "stacked/input.h"

#include <GLFW/glfw3.h>

enum class UiInputStatus
{
    NotPressed = 0,
    Pressed = 1
};

UiInput::UiInput()
{
    for (int i = 0; i < static_cast<int>(UiKeyCode::KeyCodeSize); ++i)
    {
        m_key_input[static_cast<UiKeyCode>(i)] = UiInputStatus::NotPressed;
        m_key_input_prev[static_cast<UiKeyCode>(i)] = UiInputStatus::NotPressed;
    }
}

void UiInput::Update()
{
    m_key_input_prev = m_key_input;
    m_mouse_input_prev = m_mouse_input;
}

bool UiInput::GetKey(const UiKeyCode key)
{
    return m_key_input[key] == UiInputStatus::Pressed;
}

bool UiInput::GetKeyDown(const UiKeyCode key)
{
    return m_key_input[key] == UiInputStatus::Pressed &&
           m_key_input_prev[key] == UiInputStatus::NotPressed;
}

bool UiInput::GetKeyUp(const UiKeyCode key)
{
    return m_key_input[key] == UiInputStatus::NotPressed &&
           m_key_input_prev[key] == UiInputStatus::Pressed;
}

bool UiInput::GetKeyHeld(const UiKeyCode key)
{
    return m_key_input[key] == UiInputStatus::Pressed &&
           m_key_input_prev[key] == UiInputStatus::Pressed;
}

bool UiInput::GetMouse(const UiMouseButton button)
{
    return m_mouse_input[button] == UiInputStatus::Pressed;
}

bool UiInput::GetMouseDown(const UiMouseButton button)
{
    return m_mouse_input[button] == UiInputStatus::Pressed &&
           m_mouse_input_prev[button] == UiInputStatus::NotPressed;
}

bool UiInput::GetMouseUp(const UiMouseButton button)
{
    return m_mouse_input[button] == UiInputStatus::NotPressed &&
           m_mouse_input_prev[button] == UiInputStatus::Pressed;
}

bool UiInput::GetMouseHeld(const UiMouseButton button)
{
    return m_mouse_input[button] == UiInputStatus::Pressed &&
           m_mouse_input_prev[button] == UiInputStatus::Pressed;
}

UiVec2I UiInput::GetMousePosition()
{
    return m_mouse_position;
}

void UiInput::KeyCallbackUpdate(const int glfw_keycode, const int glfw_action)
{
    const auto keycode = static_cast<UiKeyCode>(glfw_keycode);
    const auto input_status = glfw_action == GLFW_REPEAT ? UiInputStatus::Pressed : static_cast<UiInputStatus>(glfw_action);

    m_key_input[keycode] = input_status;
}

void UiInput::MouseButtonCallbackUpdate(const int glfw_button, const int glfw_action)
{
    const auto button = static_cast<UiMouseButton>(glfw_button);
    const auto input_status = glfw_action == GLFW_REPEAT ? UiInputStatus::Pressed : static_cast<UiInputStatus>(glfw_action);

    m_mouse_input[button] = input_status;
}

void UiInput::MousePositionCallbackUpdate(const double glfw_xpos, const double glfw_ypos)
{
    m_mouse_position = {static_cast<int>(glfw_xpos), static_cast<int>(glfw_ypos)};
}