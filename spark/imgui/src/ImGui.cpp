#include "spark/imgui/ImGui.h"

#include "spark/render/vk/VulkanBackend.h"
#include "spark/render/vk/VulkanCommandBuffer.h"
#include "spark/render/vk/VulkanDevice.h"
#include "spark/render/vk/VulkanRenderPass.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "GLFW/glfw3.h"

#ifndef SPARK_RELEASE
namespace
{
    constexpr VkDescriptorPoolSize IMGUI_POOL_SIZES[] =
    {
        {VK_DESCRIPTOR_TYPE_SAMPLER, 512},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 512},
        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 512},
        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 512},
        {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 512},
        {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 512},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 512},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 512},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 512},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 512},
        {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 512}
    };
    VkPipelineCache g_pipeline_cache = VK_NULL_HANDLE;
    VkDescriptorPool g_imgui_descriptor_pool = VK_NULL_HANDLE;
}

namespace spark::imgui
{
    void init(GLFWwindow* window, render::IRenderBackend& backend, render::IGraphicsDevice& device, render::IRenderPass& render_pass)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        const auto& vk_backend = dynamic_cast<render::vk::VulkanBackend&>(backend);
        const auto& vk_device = dynamic_cast<render::vk::VulkanDevice&>(device);
        const auto& vk_render_pass = dynamic_cast<render::vk::VulkanRenderPass&>(render_pass);

        // Create the ImGui descriptor pool
        constexpr VkDescriptorPoolCreateInfo pool_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets = 512,
            .poolSizeCount = static_cast<unsigned>(std::size(IMGUI_POOL_SIZES)),
            .pPoolSizes = IMGUI_POOL_SIZES
        };

        if (vkCreateDescriptorPool(vk_device.handle(), &pool_info, nullptr, &g_imgui_descriptor_pool) != VK_SUCCESS)
            throw base::NullPointerException("Failed to create ImGui descriptor pool");

        // Init ImGui for Vulkan
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {
            .Instance = vk_backend.handle(),
            .PhysicalDevice = vk_device.graphicsAdapter().handle(),
            .Device = vk_device.handle(),
            .QueueFamily = vk_device.graphicsQueue().familyId(),
            .Queue = vk_device.graphicsQueue().handle(),
            .PipelineCache = g_pipeline_cache,
            .DescriptorPool = g_imgui_descriptor_pool,
            .Subpass = 0,
            .MinImageCount = 3,
            .ImageCount = 3,
            .MSAASamples = VK_SAMPLE_COUNT_1_BIT,
            .Allocator = nullptr,
            .CheckVkResultFn = nullptr
        };

        ImGui_ImplVulkan_Init(&init_info, vk_render_pass.handle());

        const auto cmd_buffer = vk_device.graphicsQueue().createCommandBuffer(true, false);
        ImGui_ImplVulkan_CreateFontsTexture(std::as_const(*cmd_buffer).handle());
        const std::size_t fence = vk_device.graphicsQueue().submit(cmd_buffer);
        vk_device.graphicsQueue().waitFor(fence);
        ImGui_ImplVulkan_DestroyFontUploadObjects();

        log::info("ImGui initialized");
    }

    void* context()
    {
        return ImGui::GetCurrentContext();
    }

    void new_frame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void render(const render::ICommandBuffer& command_buffer)
    {
        const auto& vk_command_buffer = dynamic_cast<const render::vk::VulkanCommandBuffer&>(command_buffer);

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vk_command_buffer.handle());
    }

    void shutdown(const render::IGraphicsDevice& device)
    {
        const auto& vk_device = dynamic_cast<const render::vk::VulkanDevice&>(device);

        // Wait for the device to be idle before destroying the ImGui resources
        vk_device.wait();

        ImGui_ImplVulkan_Shutdown();
        vkDestroyDescriptorPool(vk_device.handle(), g_imgui_descriptor_pool, nullptr);

        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        log::info("ImGui shutdown");
    }
}
#else
namespace spark::imgui
{
    void init(GLFWwindow* /*window*/, render::IRenderBackend& /*backend*/, render::IGraphicsDevice& /*device*/, render::IRenderPass& /*render_pass*/) {}
    void* context() { return nullptr; }
    void new_frame() {}
    void render(const render::ICommandBuffer& /*command_buffer*/) {}
    void shutdown(const render::IGraphicsDevice& /*device*/) {}
}
#endif
