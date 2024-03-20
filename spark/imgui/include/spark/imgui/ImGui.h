#pragma once

#include "spark/imgui/Export.h"

#include "spark/render/Backend.h"
#include "spark/render/GraphicsDevice.h"
#include "spark/render/RenderPass.h"

// ReSharper disable once IdentifierTypo : Type from GLFW
// ReSharper disable once CppInconsistentNaming
struct GLFWwindow;

namespace spark::imgui
{
    /**
     * \brief Init ImGui, configures the context, the platform and the renderer.
     * \param window The window to initialize ImGui with.
     * \param backend The currently used render backend.
     * \param device The graphics device used to render ImGui.
     * \param render_pass The render pass to render ImGui with.
     */
    SPARK_IMGUI_EXPORT void init(GLFWwindow* window, render::IRenderBackend& backend, render::IGraphicsDevice& device, render::IRenderPass& render_pass);

    /**
     * \brief Gets the ImGui context.
     * \return A pointer to the ImGui context.
     *
     * This function should be used to set the current context in every library which uses ImGui in spark, before any ImGui function is called.
     * The ImGui context is static and so, not exposed outside our own boundaries. This allows to set the context.
     *
     * \note This is only required on Windows.
     */
    SPARK_IMGUI_EXPORT void* context();

    /**
     * \brief Prepares the new ImGui frame.
     */
    SPARK_IMGUI_EXPORT void new_frame();

    /**
     * \brief Renders the current frame.
     * \param command_buffer The command buffer to render the ImGui draw data.
     */
    SPARK_IMGUI_EXPORT void render(const render::ICommandBuffer& command_buffer);

    /**
     * \brief Shutdowns ImGui context, platform and renderer.
     * \param backend The graphics device with which ImGui was initialized.
     */
    SPARK_IMGUI_EXPORT void shutdown(const render::IGraphicsDevice& backend);
}
