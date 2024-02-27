#pragma once

#include "spark/math/Vector2.h"
#include "spark/render/CommandBuffer.h"
#include "spark/render/Scissor.h"
#include "spark/render/Viewport.h"

#include "glm/matrix.hpp"

#include <memory>
#include <span>
#include <string>

namespace spark::core
{
    /**
     * \brief An object that can render 2D graphics on a surface.
     * \tparam Backend The backend to use for rendering. Must be a subclass of \ref render::RenderBackend.
     */
    template <typename Backend>
    class Renderer2D
    {
    public:
        using backend_type = Backend;
        using device_type = typename backend_type::device_type;
        using buffer_type = typename device_type::buffer_type;
        using command_buffer_type = typename device_type::command_buffer_type;
        using descriptor_set_type = typename command_buffer_type::descriptor_set_type;
        using adapter_type = typename device_type::adapter_type;
        using surface_type = typename backend_type::surface_type;
        using render_pass_type = typename backend_type::render_pass_type;
        using render_pipeline_type = typename backend_type::render_pipeline_type;
        using pipeline_layout_type = typename backend_type::pipeline_layout_type;
        using shader_program_type = typename backend_type::shader_program_type;
        using shader_module_type = typename shader_program_type::shader_module_type;
        using input_assembler_type = typename backend_type::input_assembler_type;
        using rasterizer_type = typename backend_type::rasterizer_type;
        using factory_type = typename device_type::factory_type;
        using vertex_buffer_layout_type = typename factory_type::vertex_buffer_layout_type;
        using vertex_buffer_type = typename factory_type::vertex_buffer_type;
        using index_buffer_layout_type = typename factory_type::index_buffer_layout_type;
        using index_buffer_type = typename factory_type::index_buffer_type;

    public:
        /**
         * \brief Creates a new 2D renderer.
         * \param render_area The size of the area to render to.
         * \param surface_factory A factory function that creates a raw surface handle from a raw device handle.
         * \param required_extensions The list of extensions that the renderer backend requires.
         */
        explicit Renderer2D(const math::Vector2<unsigned>& render_area,
                            std::function<typename surface_type::handle_type(const typename backend_type::handle_type&)> surface_factory,
                            std::span<std::string> required_extensions);

        ~Renderer2D();

        Renderer2D(const Renderer2D& other) = delete;
        Renderer2D(Renderer2D&& other) noexcept = delete;
        Renderer2D& operator=(const Renderer2D& other) = delete;
        Renderer2D& operator=(Renderer2D&& other) noexcept = delete;

        /**
         * \brief Recreates the swap chain with frame buffers of the new \p size.
         * \param new_size The new size of the render area.
         */
        void recreateSwapChain(const math::Vector2<unsigned>& new_size);

        /**
         * \brief Draws the current frame.
         */
        void render();

        /**
         * \brief Draws a 1x1 quad with the given @p transformation_matrix.
         * \param transform_matrix The 4x4 matrix describing the transformation of the 1x1 quad into the final world space.
         * \param color The color of the quad. Defaults to white.
         */
        void drawQuad(const glm::mat4& transform_matrix, const spark::math::Vector4<float>& color = {1.f, 1.f, 1.f, 1.f});

        /**
         * \brief Draws a circle with the given @p transformation_matrix and @p radius.
         * \param transform_matrix The 4x4 matrix describing the transformation of the circle into the final world space.
         * \param radius The desired radius of the circle.
         * \param color The color of the circle. Defaults to white.
         *
         * Internally, the circle is drawn as a 1x1 quad with the given transformation matrix (in the same batch as the quads).
         * Then, the radius is passed to the fragment shader, which uses it to calculate the distance of each fragment to the center of the circle.
         * It discards the fragment if the distance is greater than the radius, effectively clipping the quad into a circle.
         */
        void drawCircle(const glm::mat4& transform_matrix, float radius, const spark::math::Vector4<float>& color = {1.f, 1.f, 1.f, 1.f});

    private:
        /**
         * \brief Init the geometry and render graph.
         */
        void initRenderGraph();

        /**
         * \brief Calculates the new camera and updates the camera buffer.
         * \param command_buffer The command buffer to use for the update.
         */
        void updateCamera(const render::ICommandBuffer& command_buffer);

        /**
         * \brief Uploads the current vertices and indices to the GPU.
         */
        void upload();

    private:
        inline static constexpr std::array s_rectangleVertices = {
            glm::vec3(-0.5f, -0.5f, 0.f),
            glm::vec3(-0.5f, 0.5f, 0.f),
            glm::vec3(0.5f, 0.5f, 0.f),
            glm::vec3(0.5f, -0.5f, 0.f)
        };

        inline static constexpr std::array<uint16_t, 6> s_rectangleIndices = {0, 1, 2, 0, 2, 3};

        device_type* m_device;
        std::unique_ptr<backend_type> m_renderBackend;
        std::shared_ptr<input_assembler_type> m_inputAssembler;
        std::unique_ptr<render::IViewport> m_viewport;
        std::unique_ptr<render::IScissor> m_scissor;
        std::vector<std::size_t> m_transferFences;

        SPARK_WARNING_PUSH
        SPARK_DISABLE_MSVC_WARNING(4324) // 'InstanceBuffer': structure was padded due to alignment specifier. This is intended to align the CPU buffer to GPU one.

        struct alignas(sizeof(glm::vec4)) InstanceBuffer
        {
            glm::mat4 transform;
            glm::vec4 color;
            float radius;
        };

        SPARK_WARNING_POP

        inline static constexpr std::size_t s_maxInstances = 1024;
        std::vector<InstanceBuffer> m_instanceData;
    };
}

#include "spark/core/impl/Renderer2D.h"
