#include <first_app.hpp>

#include <stdexcept>
#include <array>
#include <iostream>

namespace src {
    firstApp::firstApp() {
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffer();
    }

    firstApp::~firstApp() {
        vkDestroyPipelineLayout(srcDevice.srcDevice(), pipelineLayout, nullptr);
    }

    void firstApp::run() {
        while(!srcWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(srcDevice.srcDevice()); // wait for gpu operations to finish
    }

    void firstApp::loadModels() {
        std::vector<model::Vertex> vertices {
            {{0.0f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}}
        };

        srcModel = std::make_unique<model>(srcDevice, vertices);
    }

    void firstApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if(vkCreatePipelineLayout(srcDevice.srcDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout!");
        }
    }

    void firstApp::createPipeline() {
        src::PipelineConfigInfo pipelineConfig = pipeline::defaultPipelineConfigInfo(srcSwapChain.width(), srcSwapChain.height());
        pipelineConfig.renderPass = srcSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        srcPipeline = std::make_unique<pipeline>(srcDevice, "./shaders/shader.vert.spv", "./shaders/shader.frag.spv", pipelineConfig);
    }

    void firstApp::createCommandBuffer() {
        commandBuffers.resize(srcSwapChain.imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = srcDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if(vkAllocateCommandBuffers(srcDevice.srcDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers!");
        }

        for(int i = 0; i < commandBuffers.size(); ++i) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("Failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = srcSwapChain.getRenderPass();
            renderPassInfo.framebuffer = srcSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = srcSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            srcPipeline->bind(commandBuffers[i]);
            srcModel->bind(commandBuffers[i]);
            srcModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to record command buffer!");
            }
        }
    }

    void firstApp::drawFrame() {
        uint32_t imageIndex;
        VkResult result = srcSwapChain.acquireNextImage(&imageIndex);
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire next swap chain image!");
        }

        result = srcSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if(result != VK_SUCCESS) {
            throw std::runtime_error("Failed to present swap chain image!");
        }
    }
};