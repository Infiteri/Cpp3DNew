#include "Input.h"
#include "Logger.h"
#include "Window.h"
#include "Engine.h"

#include "Math/Vectors.h"

#include <GLFW/glfw3.h>

namespace Core
{
    struct InputMouseState
    {
        Vector2 position;
        Vector2 last;
        Vector2 delta;
        MouseMode lastMode;
        MouseMode mode;
    };
    // Static map to associate strings with Keys enums
    static const std::unordered_map<std::string, Keys> keyMap = {
        {"Space", Keys::Space},
        {"Apostrophe", Keys::Apostrophe},
        {"Comma", Keys::Comma},
        {"Minus", Keys::Minus},
        {"Period", Keys::Period},
        {"Slash", Keys::Slash},
        {"0", Keys::Key0},
        {"1", Keys::Key1},
        {"2", Keys::Key2},
        {"3", Keys::Key3},
        {"4", Keys::Key4},
        {"5", Keys::Key5},
        {"6", Keys::Key6},
        {"7", Keys::Key7},
        {"8", Keys::Key8},
        {"9", Keys::Key9},
        {"Semicolon", Keys::Semicolon},
        {"Equal", Keys::Equal},
        {"A", Keys::A},
        {"B", Keys::B},
        {"C", Keys::C},
        {"D", Keys::D},
        {"E", Keys::E},
        {"F", Keys::F},
        {"G", Keys::G},
        {"H", Keys::H},
        {"I", Keys::I},
        {"J", Keys::J},
        {"K", Keys::K},
        {"L", Keys::L},
        {"M", Keys::M},
        {"N", Keys::N},
        {"O", Keys::O},
        {"P", Keys::P},
        {"Q", Keys::Q},
        {"R", Keys::R},
        {"S", Keys::S},
        {"T", Keys::T},
        {"U", Keys::U},
        {"V", Keys::V},
        {"W", Keys::W},
        {"X", Keys::X},
        {"Y", Keys::Y},
        {"Z", Keys::Z},
        {"LeftBracket", Keys::LeftBracket},
        {"Backslash", Keys::Backslash},
        {"RightBracket", Keys::RightBracket},
        {"GraveAccent", Keys::GraveAccent},
        {"Escape", Keys::Escape},
        {"Enter", Keys::Enter},
        {"Tab", Keys::Tab},
        {"Backspace", Keys::Backspace},
        {"Insert", Keys::Insert},
        {"Delete", Keys::Delete},
        {"Right", Keys::Right},
        {"Left", Keys::Left},
        {"Down", Keys::Down},
        {"Up", Keys::Up},
        {"PageUp", Keys::PageUp},
        {"PageDown", Keys::PageDown},
        {"Home", Keys::Home},
        {"End", Keys::End},
        {"CapsLock", Keys::CapsLock},
        {"ScrollLock", Keys::ScrollLock},
        {"NumLock", Keys::NumLock},
        {"PrintScreen", Keys::PrintScreen},
        {"Pause", Keys::Pause},
        {"F1", Keys::F1},
        {"F2", Keys::F2},
        {"F3", Keys::F3},
        {"F4", Keys::F4},
        {"F5", Keys::F5},
        {"F6", Keys::F6},
        {"F7", Keys::F7},
        {"F8", Keys::F8},
        {"F9", Keys::F9},
        {"F10", Keys::F10},
        {"F11", Keys::F11},
        {"F12", Keys::F12},
        {"F13", Keys::F13},
        {"F14", Keys::F14},
        {"F15", Keys::F15},
        {"F16", Keys::F16},
        {"F17", Keys::F17},
        {"F18", Keys::F18},
        {"F19", Keys::F19},
        {"F20", Keys::F20},
        {"F21", Keys::F21},
        {"F22", Keys::F22},
        {"F23", Keys::F23},
        {"F24", Keys::F24},
        {"F25", Keys::F25},
        {"KP0", Keys::KP0},
        {"KP1", Keys::KP1},
        {"KP2", Keys::KP2},
        {"KP3", Keys::KP3},
        {"KP4", Keys::KP4},
        {"KP5", Keys::KP5},
        {"KP6", Keys::KP6},
        {"KP7", Keys::KP7},
        {"KP8", Keys::KP8},
        {"KP9", Keys::KP9},
        {"KPDecimal", Keys::KPDecimal},
        {"KPDivide", Keys::KPDivide},
        {"KPMultiply", Keys::KPMultiply},
        {"KPSubtract", Keys::KPSubtract},
        {"KPAdd", Keys::KPAdd},
        {"KPEnter", Keys::KPEnter},
        {"KPEqual", Keys::KPEqual},
        {"LeftShift", Keys::LeftShift},
        {"LeftControl", Keys::LeftControl},
        {"LeftAlt", Keys::LeftAlt},
        {"LeftSuper", Keys::LeftSuper},
        {"RightShift", Keys::RightShift},
        {"RightControl", Keys::RightControl},
        {"RightAlt", Keys::RightAlt},
        {"RightSuper", Keys::RightSuper},
        {"Menu", Keys::Menu}};

    static InputMouseState mouse_state;
    static Input::KeySet keys;
    static Input::KeySet justKeys;
    static Input::KeySet lastKeys;
    static std::unordered_map<Buttons, bool> buttons;

    void Input::Init()
    {
        for (auto &keyState : keys)
        {
            lastKeys[keyState.first] = keyState.second;
        }
    }

    void Input::Shutdown()
    {
    }

    bool Input::GetKey(Keys key)
    {
        return keys[key];
    }

    bool Input::GetKeyJustNow(Keys key)
    {
        return justKeys[key];
    }

    bool Input::GetButton(Buttons button)
    {
        return buttons[button];
    }

    int Input::GetMouseX()
    {
        return mouse_state.position.x;
    }

    int Input::GetMouseY()
    {
        return mouse_state.position.y;
    }

    int Input::GetMouseLastX()
    {
        return mouse_state.last.x;
    }

    int Input::GetMouseLastY()
    {
        return mouse_state.last.y;
    }

    int Input::GetMouseDeltaX()
    {
        return mouse_state.delta.x;
    }

    int Input::GetMouseDeltaY()
    {
        return mouse_state.delta.y;
    }

    int Input::GetMouseMovementDirectionX()
    {
        if (mouse_state.delta.x == 0)
            return 0;

        return mouse_state.delta.x < 0 ? -1 : 1;
    }

    int Input::GetMouseMovementDirectionY()
    {
        if (mouse_state.delta.y == 0)
            return 0;

        return mouse_state.delta.y < 0 ? -1 : 1;
    }

    void Input::SetMouseMode(MouseMode mode)
    {
        if (mouse_state.mode == mode)
            return;

        mouse_state.mode = mode;

        Window *window = Engine::GetWindow();
        if (!window)
            return;

        switch (mode)
        {
        case MouseMode::Visible:
            glfwSetInputMode(window->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;

        case MouseMode::Hidden:
            glfwSetInputMode(window->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;

        case MouseMode::Locked:
            glfwSetInputMode(window->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;

        default:
            break;
        }
    }

    Vector2 Input::GetMousePosition()
    {
        return {mouse_state.position.x, mouse_state.position.y};
    }

    Input::KeySet Input::GetKeys()
    {
        return keys;
    }

    Keys Input::KeyFromString(const std::string &key)
    {
        auto it = keyMap.find(key);
        if (it != keyMap.end())
            return it->second;

        return Keys::Key0;
    }

    void InputUpdateKey(Keys key, bool p)
    {
        lastKeys[key] = keys[key];
        keys[key] = p;
        justKeys[key] = (keys[key] && !lastKeys[key]);
    }

    void InputUpdateButton(Buttons button, bool p)
    {
        buttons[button] = p;
    }

    void InputUpdateMouse(int x, int y)
    {
        mouse_state.delta.Set(x - mouse_state.last.x, y - mouse_state.last.y);
        mouse_state.position.Set(x, y);
        mouse_state.last.Set(x, y);
    }
}