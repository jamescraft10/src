#pragma once

#include <device.hpp>
#include <model.hpp>
#include <pipeline.hpp>
#include <swap_chain.hpp>
#include <window.hpp>

#include <memory>
#include <vector>

namespace src {
    class firstApp {
        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;

            firstApp();
            ~firstApp();
            firstApp(const firstApp &) = delete;
            firstApp &operator=(const firstApp &) = delete;
            void run();
        private:
            void loadModels();
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffer();
            void drawFrame();

            window srcWindow{WIDTH, HEIGHT, "window"};
            device srcDevice{srcWindow};
            swapChain srcSwapChain{srcDevice, srcWindow.getExtent()};
            std::unique_ptr<pipeline> srcPipeline;
            VkPipelineLayout pipelineLayout;
            std::vector<VkCommandBuffer> commandBuffers;
            std::unique_ptr<model> srcModel;
    };
};