#include <window.hpp>

#include <stdexcept>

namespace src {
    window::window(int w, int h, std::string name) : width{w}, height{h}, name{name} {
        initWindow();
    }

    window::~window() {
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }

    void window::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindow = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    }

    void window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if(glfwCreateWindowSurface(instance, glfwWindow, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface");
        }
    }
};