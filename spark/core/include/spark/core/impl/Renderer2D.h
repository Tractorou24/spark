#pragma once

#include "spark/base/Exception.h"
#include "spark/lib/Pointers.h"
#include "spark/log/Logger.h"
#include "spark/path/Paths.h"
#include "spark/render/Buffer.h"
#include "spark/render/DepthStencilState.h"
#include "spark/render/Format.h"
#include "spark/render/GraphicsFactory.h"
#include "spark/render/IndexBuffer.h"
#include "spark/render/InputAssembler.h"
#include "spark/render/Rasterizer.h"
#include "spark/render/RenderTarget.h"
#include "spark/render/ShaderStages.h"

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace spark::core
{
    template <typename Backend>
    Renderer2D<Backend>::Renderer2D(const math::Vector2<unsigned>& render_area,
                                    std::function<typename surface_type::handle_type(const typename backend_type::handle_type&)> surface_factory,
                                    std::span<std::string> required_extensions)
    {
        // Setup validation layers if needed
        std::vector<std::string> layers;
#ifndef NDEBUG
        layers.emplace_back("VK_LAYER_KHRONOS_validation");
#endif

        log::info("Creating 2D renderer with a render area of {}x{}", render_area.x, render_area.y);

        // Configure the render backend
        m_renderBackend = std::make_unique<backend_type>(required_extensions, layers);
        m_viewport = std::make_unique<render::Viewport>(math::Rectangle {{}, render_area.castTo<float>()});
        m_scissor = std::make_unique<render::Scissor>(math::Rectangle {{}, render_area.castTo<float>()});

        // Query the available graphics adapters and find a suitable one
        const adapter_type* selected_adapter = m_renderBackend->findAdapter(std::nullopt);
        if (!selected_adapter)
            throw base::NullPointerException("No suitable graphics adapter found");

        // Create the surface and the device
        auto surface = m_renderBackend->createSurface(surface_factory);
        m_device = m_renderBackend->template createDevice<backend_type>("Default",
                                                                        *selected_adapter,
                                                                        std::move(surface),
                                                                        render::Format::B8G8R8A8_UNORM,
                                                                        m_viewport->rectangle().extent.castTo<unsigned>(),
                                                                        3);

        // Vertex and index buffer layouts
        auto vertex_buffer_layout = std::make_unique<vertex_buffer_layout_type>(sizeof(glm::vec3), 0);
        vertex_buffer_layout->addAttribute(render::BufferAttribute(0, 0, render::BufferFormat::XYZ32F, render::AttributeSemantic::Position, 0));

        std::vector<std::unique_ptr<vertex_buffer_layout_type>> vertex_buffer_layouts;
        vertex_buffer_layouts.push_back(std::move(vertex_buffer_layout));

        auto index_buffer_layout = std::make_unique<index_buffer_layout_type>(render::IndexType::UInt16);

        // Setup input assembler
        m_inputAssembler = std::make_shared<input_assembler_type>(std::move(vertex_buffer_layouts), std::move(index_buffer_layout), render::PrimitiveTopology::TriangleList);

        // Create a simple render pass
        std::vector<render::RenderTarget> render_targets;
        render_targets.emplace_back("Color Target",
                                    0,
                                    render::RenderTargetType::Present,
                                    render::Format::B8G8R8A8_UNORM,
                                    true,
                                    math::Vector4 {0.2f, 0.2f, 0.2f, 1.f},
                                    false,
                                    false);
        render_targets.emplace_back("Depth/Stencil Target",
                                    1,
                                    render::RenderTargetType::DepthStencil,
                                    render::Format::D32_SFLOAT,
                                    true,
                                    math::Vector4 {1.f, 1.f, 1.f, 1.f},
                                    false,
                                    false);

        auto render_pass = std::make_unique<render_pass_type>(*m_device, "Opaque", render_targets);

        // Create the shader program
        std::vector<std::unique_ptr<shader_module_type>> modules;
        modules.push_back(std::make_unique<shader_module_type>(*m_device, render::ShaderStage::Vertex, spark::path::engine_assets_path() / "shaders" / "2d_vert.spv"));
        modules.push_back(std::make_unique<shader_module_type>(*m_device, render::ShaderStage::Fragment, spark::path::engine_assets_path() / "shaders" / "2d_frag.spv"));

        auto shader_program = std::make_shared<shader_program_type>(*m_device, std::move(modules));

        // Create the rasterizer
        render::DepthStencilState depth_stencil_state(render::DepthStencilState::DepthState {.operation = render::CompareOperation::LessEqual},
                                                      render::DepthStencilState::DepthBias {},
                                                      render::DepthStencilState::StencilState {});
        auto rasterizer = std::make_shared<rasterizer_type>(render::PolygonMode::Solid, render::CullMode::BackFaces, render::CullOrder::CounterClockWise, 1.f, depth_stencil_state);

        // Create the render pipeline
        auto render_pipeline = std::make_unique<render_pipeline_type>(*render_pass,
                                                                      shader_program,
                                                                      std::static_pointer_cast<pipeline_layout_type>(shader_program->reflectPipelineLayout()),
                                                                      m_inputAssembler,
                                                                      rasterizer,
                                                                      false,
                                                                      "Geometry");

        // Add everything to the device state
        m_device->state().add(std::move(render_pass));
        m_device->state().add(std::move(render_pipeline));

        // Init the render graph
        initRenderGraph();
    }

    template <typename Backend>
    Renderer2D<Backend>::~Renderer2D()
    {
        m_renderBackend->releaseDevice("Default");
    }

    template <typename Backend>
    void Renderer2D<Backend>::recreateSwapChain(const math::Vector2<unsigned>& new_size)
    {
        // Wait for all frames to be finished
        m_device->wait();

        // Recreate the swap chain
        const auto surface_format = m_device->swapChain().surfaceFormat();
        m_device->swapChain().reset(surface_format, new_size, 3);

        // Resize the frame buffers for the render passes. It should be done in order to avoid since dependencies
        // (i.e. input attachments) are re-created and might be mapped to images that do no longer exist.
        m_device->state().renderPass("Opaque").resizeFrameBuffers(new_size);

        // Resize viewport and scissor.
        m_viewport->setRectangle(math::Rectangle({0.f, 0.f}, new_size.castTo<float>()));
        m_scissor->setRectangle(math::Rectangle({0.f, 0.f}, new_size.castTo<float>()));
    }

    template <typename Backend>
    void Renderer2D<Backend>::initRenderGraph()
    {
        auto& render_pipeline = m_device->state().pipeline("Geometry");
        auto command_buffer = m_device->transferQueue().createCommandBuffer(true, false);

        // Create the vertex buffer
        auto staged_vertex_buffer = m_device->factory().createVertexBuffer(m_inputAssembler->vertexBufferLayout(0),
                                                                           render::BufferUsage::Staging,
                                                                           static_cast<unsigned>(s_rectangleVertices.size()));

        auto vertex_buffer = m_device->factory().createVertexBuffer("Vertex Buffer",
                                                                    m_inputAssembler->vertexBufferLayout(0),
                                                                    render::BufferUsage::Resource,
                                                                    static_cast<unsigned>(s_rectangleVertices.size()));

        staged_vertex_buffer->map(s_rectangleVertices.data(), s_rectangleVertices.size() * sizeof(glm::vec3), 0);
        command_buffer->transfer(std::move(staged_vertex_buffer), *vertex_buffer, 0, 0, static_cast<unsigned>(s_rectangleVertices.size()));

        // Create the index buffer
        auto staged_indices_buffer = m_device->factory().createIndexBuffer(m_inputAssembler->indexBufferLayout(),
                                                                           render::BufferUsage::Staging,
                                                                           static_cast<unsigned>(s_rectangleIndices.size()));

        auto index_buffer = m_device->factory().createIndexBuffer("Index Buffer",
                                                                  m_inputAssembler->indexBufferLayout(),
                                                                  render::BufferUsage::Resource,
                                                                  static_cast<unsigned>(s_rectangleIndices.size()));

        staged_indices_buffer->map(s_rectangleIndices.data(), s_rectangleIndices.size() * sizeof(uint16_t), 0);
        command_buffer->transfer(std::move(staged_indices_buffer), *index_buffer, 0, 0, static_cast<unsigned>(s_rectangleIndices.size()));

        // Create the transform bindings and buffer
        auto& instance_binding_layout = render_pipeline.layout()->descriptorSet(0);

        // Since we are using an unstructured storage buffer, we need to specify the element size manually.
        auto staged_instance_buffer = lib::dynamic_unique_pointer_cast<buffer_type>(dynamic_cast<const render::IGraphicsFactory&>(m_device->factory()).
                                                                                    createBuffer("Instance Staging Buffer",
                                                                                                 instance_binding_layout,
                                                                                                 0,
                                                                                                 render::BufferUsage::Staging,
                                                                                                 sizeof(InstanceBuffer),
                                                                                                 s_maxInstances));

        auto instance_buffer = lib::dynamic_unique_pointer_cast<buffer_type>(dynamic_cast<const render::IGraphicsFactory&>(m_device->factory()).createBuffer("Instance Buffer",
                                                                                      instance_binding_layout,
                                                                                      0,
                                                                                      render::BufferUsage::Resource,
                                                                                      sizeof(InstanceBuffer),
                                                                                      s_maxInstances));

        auto instance_binding = instance_binding_layout.allocate(s_maxInstances, {{0, *instance_buffer}});

        m_transferFences.push_back(m_device->transferQueue().submit(command_buffer));

        m_device->state().add(lib::static_unique_pointer_cast<render::IVertexBuffer>(std::move(vertex_buffer)));
        m_device->state().add(lib::static_unique_pointer_cast<render::IIndexBuffer>(std::move(index_buffer)));
        m_device->state().add(std::move(staged_instance_buffer));
        m_device->state().add(std::move(instance_buffer));
        m_device->state().add("Instance Binding", std::move(instance_binding));
    }

    template <typename Backend>
    void Renderer2D<Backend>::updateCamera(const render::ICommandBuffer& command_buffer)
    {
        // Calculate the view matrix
        const glm::mat4 view = glm::ortho(0.f, m_viewport->rectangle().extent.x, m_viewport->rectangle().extent.y, 0.f, -1.f, 1.f);

        auto& pipeline = dynamic_cast<render_pipeline_type&>(m_device->state().pipeline("Geometry"));
        command_buffer.pushConstants(*pipeline.layout()->pushConstants(), &view);
    }

    template <typename Backend>
    void Renderer2D<Backend>::upload()
    {
        if (m_instanceData.empty()) // Nothing to upload
            return;

        // Create a command buffer
        auto command_buffer = m_device->transferQueue().createCommandBuffer(true, false);

        // Transfer frame data to the instance buffer
        auto& staged_instance_buffer = dynamic_cast<buffer_type&>(m_device->state().buffer("Instance Staging Buffer"));
        staged_instance_buffer.map(static_cast<const void*>(m_instanceData.data()), m_instanceData.size() * sizeof(InstanceBuffer), 0);

        command_buffer->transfer(staged_instance_buffer,
                                 dynamic_cast<buffer_type&>(m_device->state().buffer("Instance Buffer")),
                                 0,
                                 0,
                                 static_cast<unsigned>(m_instanceData.size()));

        // Submit the command buffer
        m_transferFences.push_back(m_device->transferQueue().submit(command_buffer));
    }

    template <typename Backend>
    void Renderer2D<Backend>::render()
    {
        // Swap the back buffers for the next frame
        const auto back_buffer = m_device->swapChain().swapBackBuffer();

        // TODO: Cache this instead of looking them up every frame
        auto& render_pass = m_device->state().renderPass("Opaque");
        const auto& geometry_pipeline = m_device->state().pipeline("Geometry");
        const auto& instance_binding = m_device->state().descriptorSet("Instance Binding");
        const auto& vertex_buffer = m_device->state().vertexBuffer("Vertex Buffer");
        const auto& index_buffer = m_device->state().indexBuffer("Index Buffer");

        // Upload the new instance data for the current frame
        upload();

        // Begin rendering on the render pass and use the only pipeline created for it
        render_pass.begin(back_buffer);

        // Wait for all transfers to finish
        for (const auto& fence : m_transferFences)
            m_device->transferQueue().waitFor(fence);
        m_transferFences.clear();

        const auto command_buffer = render_pass.activeFrameBuffer().commandBuffer(0);
        command_buffer->use(geometry_pipeline);
        command_buffer->setViewport(m_viewport.get());
        command_buffer->setScissor(m_scissor.get());

        // Set up the camera
        updateCamera(*command_buffer);

        // Bind the vertex and index buffers
        command_buffer->bind(instance_binding);
        command_buffer->bind(vertex_buffer);
        command_buffer->bind(index_buffer);

        // Draw the object and present the frame by ending the render pass
        command_buffer->drawIndexed(index_buffer.elements(), static_cast<unsigned>(m_instanceData.size()));
        render_pass.end();

        // Clean up the instance data for the next frame
        m_instanceData.clear();
    }

    template <typename Backend>
    void Renderer2D<Backend>::drawQuad(const glm::mat4& transform_matrix, const spark::math::Vector4<float>& color)
    {
        m_instanceData.emplace_back(InstanceBuffer {
                                        .transform = transform_matrix,
                                        .color = glm::vec4(color.x, color.y, color.z, color.w),
                                    });
    }
}
