#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace src {
    class window {
        private:
            void initWindow();
            const int width;
            const int height;
            std::string name;
            GLFWwindow *glfwWindow;
        public:
            window(int w, int h, std::string name);
            ~window();
            window(const window &) = delete;
            window &operator=(const window &) = delete;

            bool shouldClose() { return glfwWindowShouldClose(glfwWindow); }
            VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
            void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    };
};