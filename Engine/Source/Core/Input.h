#pragma once

#include "Base.h"

namespace Core
{
    /// @brief Keys
    enum class Keys
    {
        // Printable keys
        Space = 32,
        Apostrophe = 39, // '
        Comma = 44,      // ,
        Minus = 45,      // -
        Period = 46,     // .
        Slash = 47,      // /
        Key0 = 48,
        Key1 = 49,
        Key2 = 50,
        Key3 = 51,
        Key4 = 52,
        Key5 = 53,
        Key6 = 54,
        Key7 = 55,
        Key8 = 56,
        Key9 = 57,
        Semicolon = 59, // ;
        Equal = 61,     // =
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
        LeftBracket = 91,  // [
        Backslash = 92,    //
        RightBracket = 93, // ]
        GraveAccent = 96,  // `

        // Function keys
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,

        // Keypad
        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,
        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,

        // Modifiers
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,

        // Miscellaneous
        Menu = 348
    };

    enum class Buttons
    {
        Left = 0,
        Right = 1,
        Middle = 2
    };

    enum class MouseMode
    {
        Visible,
        Hidden,
        Locked
    };

    class CE_API Input
    {
    public:
        Input(){};
        ~Input(){};

        static void Init();
        static void Shutdown();

        /// @brief If true, the passed in key is being held down.
        /// @param key A key from the Core::Keys enum to check for.
        /// @return True or False.
        static bool GetKey(Keys key);

        /// @brief If true, the passed in key has just been pressed.
        /// @param key A key from the Core::Keys enum to check for.
        /// @return True or False
        static bool GetKeyJustNow(Keys key);

        /// @brief If true, the passed in button is being held down.
        /// @param button A button from the Core::Buttons enum to check for.
        /// @return True or False.
        static bool GetButton(Buttons button);

        /// @brief Returns the mouse X position.
        /// @return Integer.
        static int GetMouseX();

        /// @brief Returns the mouse Y position.
        /// @return Integer.
        static int GetMouseY();

        /// @brief Returns the mouse last X position.
        /// @return Integer.
        static int GetMouseLastX();

        /// @brief Returns the mouse last Y position.
        /// @return Integer.
        static int GetMouseLastY();

        /// @brief Returns the mouse delta X, which is the current X position - the last X position.
        /// @return Integer.
        static int GetMouseDeltaX();

        /// @brief Returns the mouse delta Y, which is the current Y position - the last Y position.
        /// @return Integer.
        static int GetMouseDeltaY();

        /// @brief Returns the mouse movement direction on the X axis, from -1 to 1.
        /// @return Integer.
        static int GetMouseMovementDirectionX();

        /// @brief Returns the mouse movement direction on the Y axis, from -1 to 1.
        /// @return Integer.
        static int GetMouseMovementDirectionY();

        /// @brief Sets the mouse mode to the specified mode.
        /// @param mode The mode of the mouse.
        static void SetMouseMode(MouseMode mode);
    };

    void InputUpdateKey(Keys key, bool p);
    void InputUpdateButton(Buttons button, bool p);
    void InputUpdateMouse(int x, int y);
}