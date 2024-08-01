/**
 * input.h
 */

#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>

enum class KeyCode
{
    SPACEBAR = 32,
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
    TILDE = 96,
    TAB = 258,
    ARROW_RIGHT = 262,
    ARROW_LEFT = 263,
    ARROW_DOWN = 264,
    ARROW_UP = 265,
    ESCAPE = 256,
    CAPSLOCK = 280,
    LSHIFT = 340,
    LCTRL = 341,
    KEYCODE_SIZE
};

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
    * \brief Initialises the input system by setting relevant keys to NOT_PRESSED.
    */
    static void Initialise();

    /**
     * \brief Called when the handling the GLFW key callback.
     * \param glfw_keycode The GLFW key code for the key event.
     * \param glfw_action The GLFW action for the key event.
     */
    static void CallbackUpdate(int glfw_keycode, int glfw_action);

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

private:
    Input() = default;
    ~Input() = default;

    static Input& Get();
    static Input s_instance;

    std::unordered_map<KeyCode, InputStatus> m_input_map;
    std::unordered_map<KeyCode, InputStatus> m_input_map_prev;
};

#endif // INPUT_H
