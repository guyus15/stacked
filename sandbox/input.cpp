/**
 * input.cpp
 */

#include "input.h"

#include <GLFW/glfw3.h>

enum class InputStatus
{
    NOT_PRESSED = 0,
    PRESSED = 1
};

Input Input::s_instance{};

/**
 * \brief Initialises the input system by setting relevant keys to NOT_PRESSED.
 */
void Input::Initialise()
{
    for (int i = 0; i < static_cast<int>(KeyCode::KEYCODE_SIZE); ++i)
    {
        Get().m_input_map[static_cast<KeyCode>(i)] = InputStatus::NOT_PRESSED;
        Get().m_input_map_prev[static_cast<KeyCode>(i)] = InputStatus::NOT_PRESSED;
    }
}

/**
 * \brief Called when the handling the GLFW key callback.
 * \param glfw_keycode The GLFW key code for the key event.
 * \param glfw_action The GLFW action for the key event.
 */
void Input::CallbackUpdate(const int glfw_keycode, int glfw_action)
{
    const auto key_code = static_cast<KeyCode>(glfw_keycode);
    const auto input_status = glfw_action == GLFW_REPEAT ? InputStatus::PRESSED :
                                  static_cast<InputStatus>(glfw_action);

    Get().m_input_map[key_code] = input_status;
}

/**
 * \brief Called each frame of the application.
 * \note This must be placed at the end of your main loop,
 * but before glfwPollEvents().
 */
void Input::Update()
{
    Get().m_input_map_prev = Get().m_input_map;
}

/**
 * \brief Determines if the given key is being pressed.
 * \param key The given key.
 * \return True or false value depending on whether the given key is being pressed.
 */
bool Input::GetKey(const KeyCode key)
{
    return Get().m_input_map[key] == InputStatus::PRESSED;
}

/**
 * \brief Determines if the given key has been pressed down in the current frame.
 * \param key The given key.
 * \return True or false value depending on whether the given key has been pressed
 * down in the current frame.
 */
bool Input::GetKeyDown(const KeyCode key)
{
    return Get().m_input_map[key] == InputStatus::PRESSED &&
           Get().m_input_map_prev[key] == InputStatus::NOT_PRESSED;
}

/**
 * \brief Determines if the given key has been released in the current frame.
 * \param key The given key.
 * \return True or false value depending on whether the given key has been
 * released in the current frame.
 */
bool Input::GetKeyUp(const KeyCode key)
{
    return Get().m_input_map[key] == InputStatus::NOT_PRESSED &&
        Get().m_input_map_prev[key] == InputStatus::PRESSED;
}

/**
 * \brief Determines if the given key is being held down by looking at the state
 * of the key in the previous and current frame.
 * \param key The given key.
 * \return True or false value depending on whether the given key is being
 * held down.
 */
bool Input::GetKeyHeld(const KeyCode key)
{
    return Get().m_input_map[key] == InputStatus::PRESSED &&
        Get().m_input_map_prev[key] == InputStatus::PRESSED;
}

Input& Input::Get()
{
    return s_instance;
}
