/**
 * input.h
 */

#ifndef INPUT_H
#define INPUT_H

#include "types.h"

#include <unordered_map>

struct GLFWwindow;

enum class KeyCode
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

enum class MouseButton
{
    LeftMouse = 0,
    RightMouse = 1
};

enum class InputStatus;

class Input
{
public:
    /**
     * \brief Initialises the input system by setting relevant keys to NotPressed.
     */
    static void Initialise();

    /**
     * \brief Called each frame of the application.
     */
    static void Update();

    /**
     * \brief Determines if the given key is being pressed.
     * \param key The given key.
     * \return True or false value depending on whether the given key is being pressed.
     */
    static bool GetKey(KeyCode key);

    /**
     * \brief Determines if the given key has been pressed down in the current frame.
     * \param key The given key.
     * \return True or false value depending on whether the given key has been pressed
     * down in the current frame.
     */
    static bool GetKeyDown(KeyCode key);

    /**
     * \brief Determines if the given key has been released in the current frame.
     * \param key The given key.
     * \return True or false value depending on whether the given key has been
     * released in the current frame.
     */
    static bool GetKeyUp(KeyCode key);

    /**
     * \brief Determines if the given key is being held down by looking at the state
     * of the key in the previous and current frame.
     * \param key The given key.
     * \return True or false value depending on whether the given key is being
     * held down.
     */
    static bool GetKeyHeld(KeyCode key);

    /**
     * \brief Determines if the given mouse button is being pressed.
     * \return True or false depending on whether the given mouse button is being pressed.
     */
    static bool GetMouse(MouseButton button);

    /**
     * \brief Determines if the given mouse button has been pressed down in the current frame.
     * \return True or false depending on whether the given mouse button has been pressed down
     * in the current frame.
     */
    static bool GetMouseDown(MouseButton key);

    /**
     * \brief Determines if the given mouse button has been released in the current frame.
     * \return True or false depending on whether the given mouse button has been released
     * in the current frame.
     */
    static bool GetMouseUp(MouseButton key);

    /**
     * \brief Determines if the given mouse button is being held down by looking at the state
     * of the key in the previous and current frame.
     * \return True or false depending on whether the given mouse button is being held down.
     */
    static bool GetMouseHeld(MouseButton key);

    /**
     * \brief Gets the current mouse position.
     * \return The mouse's current position.
     */
    static UiVec2I GetMousePosition();

private:
    Input() = default;

    static Input &Get();
    static Input s_instance;

    std::unordered_map<KeyCode, InputStatus> m_key_input;
    std::unordered_map<KeyCode, InputStatus> m_key_input_prev;
    std::unordered_map<MouseButton, InputStatus> m_mouse_input;
    std::unordered_map<MouseButton, InputStatus> m_mouse_input_prev;
    UiVec2I m_mouse_position;

    /**
     * \brief Called when the handling the GLFW key callback.
     * \param glfw_keycode The GLFW key code for the key event.
     * \param glfw_action The GLFW action for the key event.
     */
    static void KeyCallbackUpdate(int glfw_keycode, int glfw_action);

    /**
     * \brief Called when handling the GLFW mouse position callback.
     * \param glfw_xpos The mouse's X position.
     * \param glfw_ypos The mouse's Y position.
     */
    static void MouseButtonCallbackUpdate(int glfw_button, int glfw_action);

    /**
     * \brief Called when handling the GLFW mouse button callback.
     * \param glfw_button The mouse button.
     * \param glfw_action The action of the mouse button.
     */
    static void MousePositionCallbackUpdate(double glfw_xpos, double glfw_ypos);

    friend void KeyCallback(GLFWwindow *, int, int, int, int);
    friend void MouseButtonCallback(GLFWwindow *, int, int, int);
    friend void MousePositionCallback(GLFWwindow *, double, double);
};

void KeyCallback(GLFWwindow *, int, int, int, int);
void MouseButtonCallback(GLFWwindow *, int, int, int);
void MousePositionCallback(GLFWwindow *, double, double);

#endif // INPUT_H