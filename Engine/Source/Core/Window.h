#pragma once

#include "Base.h"

typedef struct GLFWwindow GLFWwindow;

namespace Core
{
    class Window
    {
    public:
        /// @brief Defines all the different size modes.
        enum SizeModes
        {
            /// @brief Represents a normal type window, with the topbar and sizes (X, Y, Width, height) respected.
            Normal,

            /// @brief Represents a windowed window. The sizes are not taken into consideration.
            Windowed,

            /// @brief Represents a windowed window. The sizes are not taken into consideration. No topbar.
            Fullscreen
        };

        struct Information
        {
            int X = 100;
            int Y = 100;
            int Width = 1280;
            int Height = 720;
            const char *Title = "Window Title";
            bool VSync = true;
            bool Decorated = true;
            SizeModes SizeMode = Normal;
        };

    private:
        Information info;
        GLFWwindow *handle;

    public:
        Window(const Information &_Info);
        ~Window();

        void Update();
        bool ShouldRun();

        inline float GetWidth() { return info.Width; };
        inline float GetHeight() { return info.Height; };

        Information *GetInfo() { return &info; };
        GLFWwindow *GetHandle() { return handle; };
    };
}