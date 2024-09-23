/**
 * input.h
 */

#ifndef INPUT_H
#define INPUT_H

#include "stacked/types.h"

#include <unordered_map>

struct GLFWwindow;

enum class UiKeyCode
{
    Spacebar = 32,
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    Tilde = 96,
    Tab = 258,
    ArrowRight = 262,
    ArrowLeft = 263,
    ArrowDown = 264,
    ArrowUp = 265,
    Escape = 256,
    CapsLock = 280,
    LeftShift = 340,
    LeftCtrl = 341,
    KeyCodeSize
};

enum class UiMouseButton
{
    LeftMouse = 0,
    RightMouse = 1
};

enum class UiInputStatus;

class UiInput
{
public:
    UiInput();

    /**
     * \brief Called each frame of the application.
     */
    void Update();

    /**
     * \brief Determines if the given key is being pressed.
     * \param key The given key.
     * \return True or false value depending on whether the given key is being pressed.
     */
    bool GetKey(UiKeyCode key);

    /**
     * \brief Determines if the given key has been pressed down in the current frame.
     * \param key The given key.
     * \return True or false value depending on whether the given key has been pressed
     * down in the current frame.
     */
    bool GetKeyDown(UiKeyCode key);

    /**
     * \brief Determines if the given key has been released in the current frame.
     * \param key The given key.
     * \return True or false value depending on whether the given key has been
     * released in the current frame.
     */
    bool GetKeyUp(UiKeyCode key);

    /**
     * \brief Determines if the given key is being held down by looking at the state
     * of the key in the previous and current frame.
     * \param key The given key.
     * \return True or false value depending on whether the given key is being
     * held down.
     */
    bool GetKeyHeld(UiKeyCode key);

    /**
     * \brief Determines if the given mouse button is being pressed.
     * \return True or false depending on whether the given mouse button is being pressed.
     */
    bool GetMouse(UiMouseButton button);

    /**
     * \brief Determines if the given mouse button has been pressed down in the current frame.
     * \return True or false depending on whether the given mouse button has been pressed down
     * in the current frame.
     */
    bool GetMouseDown(UiMouseButton key);

    /**
     * \brief Determines if the given mouse button has been released in the current frame.
     * \return True or false depending on whether the given mouse button has been released
     * in the current frame.
     */
    bool GetMouseUp(UiMouseButton key);

    /**
     * \brief Determines if the given mouse button is being held down by looking at the state
     * of the key in the previous and current frame.
     * \return True or false depending on whether the given mouse button is being held down.
     */
    bool GetMouseHeld(UiMouseButton key);

    /**
     * \brief Gets the current mouse position.
     * \return The mouse's current position.
     */
    UiVec2I GetMousePosition();

    /**
     * \brief Called when the handling the GLFW key callback.
     * \param glfw_keycode The GLFW key code for the key event.
     * \param glfw_action The GLFW action for the key event.
     */
    void KeyCallbackUpdate(int glfw_keycode, int glfw_action);

    /**
     * \brief Called when handling the GLFW mouse position callback.
     * \param glfw_xpos The mouse's X position.
     * \param glfw_ypos The mouse's Y position.
     */
    void MouseButtonCallbackUpdate(int glfw_button, int glfw_action);

    /**
     * \brief Called when handling the GLFW mouse button callback.
     * \param glfw_button The mouse button.
     * \param glfw_action The action of the mouse button.
     */
    void MousePositionCallbackUpdate(double glfw_xpos, double glfw_ypos);

private:
    std::unordered_map<UiKeyCode, UiInputStatus> m_key_input;
    std::unordered_map<UiKeyCode, UiInputStatus> m_key_input_prev;
    std::unordered_map<UiMouseButton, UiInputStatus> m_mouse_input;
    std::unordered_map<UiMouseButton, UiInputStatus> m_mouse_input_prev;
    UiVec2I m_mouse_position;
};

#endif // INPUT_H