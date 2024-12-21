#include "Window.h"
#include "Logger.h"
#include "Input.h"

#include "Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Core
{
    double accumulatedScrollX = 0.0;
    double accumulatedScrollY = 0.0;

    static void Viewport(GLFWwindow *window, int w, int h);
    static void WindowOnScroll(GLFWwindow *w, double x, double y);

    Window::Window(const Information &_Info)
    {
        info = _Info;

        glfwInit();
        glfwWindowHint(GLFW_VISIBLE, false);
        glfwWindowHint(GLFW_DECORATED, info.Decorated);
        glfwWindowHint(GLFW_SAMPLES, 4);

        // Setup monitor
        GLFWmonitor *Monitor = NULL;
        {
            if (info.SizeMode == Fullscreen)
            {
                Monitor = glfwGetPrimaryMonitor();
                info.Width = glfwGetVideoMode(Monitor)->width;
                info.Height = glfwGetVideoMode(Monitor)->height;
            }
        }

        handle = glfwCreateWindow(info.Width, info.Height, info.Title, Monitor, NULL);
        glfwMakeContextCurrent(handle);
        glfwSetWindowPos(handle, info.X, info.Y);
        glfwShowWindow(handle);

        if (info.SizeMode == Windowed)
            glfwMaximizeWindow(handle);

        // Handle VSYnc
        if (info.VSync)
            glfwSwapInterval(1);

        glfwSetWindowSizeCallback(handle, Viewport);

        //! Update state.
        glfwGetWindowPos(handle, &info.X, &info.Y);
        glfwGetWindowSize(handle, &info.Width, &info.Height);
    }

    Window::~Window()
    {
        glfwDestroyWindow(handle);
        glfwTerminate();
    }

    void Window::Update()
    {
        glfwGetWindowPos(handle, &info.X, &info.Y);
        glfwGetWindowSize(handle, &info.Width, &info.Height);

        glfwPollEvents();
        glfwSwapBuffers(handle);
        glfwSetScrollCallback(handle, WindowOnScroll);

        {
            double dx, dy;
            glfwGetCursorPos(handle, &dx, &dy);
            InputUpdateMouse((int)dx, (int)dy);

            for (int key = 0; key < GLFW_KEY_LAST; key++)
                InputUpdateKey((Keys)key, glfwGetKey(handle, key) != 0);

            for (int button = 0; button < GLFW_MOUSE_BUTTON_LAST; button++)
                InputUpdateButton((Buttons)button, glfwGetMouseButton(handle, button) != 0);

            InputUpdateScroll(accumulatedScrollX, accumulatedScrollY);
            accumulatedScrollX = accumulatedScrollY = 0;
        }
    }

    bool Window::ShouldRun()
    {
        return !glfwWindowShouldClose(handle);
    }

    void Viewport(GLFWwindow *window, int w, int h)
    {
        Renderer::Viewport(w, h);
    }

    void WindowOnScroll(GLFWwindow *w, double x, double y)
    {
        accumulatedScrollX += x;
        accumulatedScrollY += y;
    };

}