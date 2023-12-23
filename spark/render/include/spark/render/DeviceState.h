#pragma once

#include "spark/render/Buffer.h"
#include "spark/render/DescriptorSet.h"
#include "spark/render/Export.h"
#include "spark/render/Image.h"
#include "spark/render/IndexBuffer.h"
#include "spark/render/Pipeline.h"
#include "spark/render/RenderPass.h"
#include "spark/render/Sampler.h"
#include "spark/render/VertexBuffer.h"

#include <memory>
#include <string>

namespace spark::render
{
    /**
     * \brief A class used to manage the state of a \ref IGraphicsDevice.
     *
     * The device state makes managing resources created by a device easier, since you do not have to worry about storage and release order.
     * However, that this is not free. Requesting a resource requires a lookup within a hash-map. Also device states are not specialized for the 
     * concrete device, so you can only work with interfaces. This implies potentially inefficient upcasting of the state resource when its passed to 
     * another object. You have to decide if or to which degree you want to rely on storing resources in a device state.
     */
    class SPARK_RENDER_EXPORT DeviceState final
    {
    public:
        /**
         * \brief Creates a new \ref DeviceState instance.
         */
        explicit DeviceState();
        ~DeviceState();

        DeviceState(const DeviceState& other) = delete;
        DeviceState(DeviceState&& other) noexcept = delete;
        DeviceState& operator=(const DeviceState& other) = delete;
        DeviceState& operator=(DeviceState&& other) noexcept = delete;

        /**
         * \brief Gets a \ref IRenderPass from the device state.
         * \param id The identifier of the render pass.
         * \return A reference to the \ref RenderPass associated with the given identifier.
         */
        [[nodiscard]] IRenderPass& renderPass(const std::string& id) const;

        /**
         * \brief Gets a \ref IPipeline from the device state.
         * \param id The identifier of the pipeline.
         * \return A reference to the \ref IPipeline associated with the given identifier.
         */
        [[nodiscard]] IPipeline& pipeline(const std::string& id) const;

        /**
         * \brief Gets a \ref IBuffer from the device state.
         * \param id The identifier of the buffer.
         * \return A reference to the \ref IBuffer associated with the given identifier.
         */
        [[nodiscard]] IBuffer& buffer(const std::string& id) const;

        /**
         * \brief Gets a \ref IVertexBuffer from the device state.
         * \param id The identifier of the vertex buffer.
         * \return A reference to the \ref IVertexBuffer associated with the given identifier.
         */
        [[nodiscard]] IVertexBuffer& vertexBuffer(const std::string& id) const;

        /**
         * \brief Gets a \ref IIndexBuffer from the device state.
         * \param id The identifier of the index buffer.
         * \return A reference to the \ref IIndexBuffer associated with the given identifier.
         */
        [[nodiscard]] IIndexBuffer& indexBuffer(const std::string& id) const;

        /**
         * \brief Gets a \ref IImage from the device state.
         * \param id The identifier of the image.
         * \return Gets a reference to the \ref IImage associated with the given identifier.
         */
        [[nodiscard]] IImage& image(const std::string& id) const;

        /**
         * \brief Gets a \ref ISampler from the device state.
         * \param id The identifier of the sampler.
         * \return A reference to the \ref ISampler associated with the given identifier.
         */
        [[nodiscard]] ISampler& sampler(const std::string& id) const;

        /**
         * \brief Gets a \ref IDescriptorSet from the device state.
         * \param id The identifier of the descriptor set.
         * \return A reference to the \ref IDescriptorSet associated with the given identifier.
         */
        [[nodiscard]] IDescriptorSet& descriptorSet(const std::string& id) const;

        /**
         * \brief Adds a \ref IRenderPass to the device state and uses its name as identifier.
         * \param render_pass A \ref std::unique_ptr to the \ref IRenderPass to add.
         */
        void add(std::unique_ptr<IRenderPass>&& render_pass);

        /**
         * \brief Adds a new \ref IRenderPass to the device state.
         * \param id The identifier of the render pass in the device state.
         * \param render_pass A \ref std::unique_ptr to the \ref IRenderPass to add.
         */
        void add(const std::string& id, std::unique_ptr<IRenderPass>&& render_pass);

        /**
         * \brief Adds a \ref IPipeline to the device state and uses its name as identifier.
         * \param pipeline A \ref std::unique_ptr to the \ref IPipeline to add.
         */
        void add(std::unique_ptr<IPipeline>&& pipeline);

        /**
         * \brief Adds a new \ref IPipeline to the device state.
         * \param id The identifier of the pipeline in the device state.
         * \param pipeline A \ref std::unique_ptr to the \ref IPipeline to add.
         */
        void add(const std::string& id, std::unique_ptr<IPipeline>&& pipeline);

        /**
         * \brief Adds a \ref IBuffer to the device state and uses its name as identifier.
         * \param buffer A \ref std::unique_ptr to the \ref IBuffer to add.
         */
        void add(std::unique_ptr<IBuffer>&& buffer);

        /**
         * \brief Adds a new \ref IBuffer to the device state.
         * \param id The identifier of the buffer in the device state.
         * \param buffer A \ref std::unique_ptr to the \ref IBuffer to add.
         */
        void add(const std::string& id, std::unique_ptr<IBuffer>&& buffer);

        /**
         * \brief Adds a \ref IVertexBuffer to the device state and uses its name as identifier.
         * \param vertex_buffer A \ref std::unique_ptr to the \ref IVertexBuffer to add.
         */
        void add(std::unique_ptr<IVertexBuffer>&& vertex_buffer);

        /**
         * \brief Adds a new \ref IVertexBuffer to the device state.
         * \param id The identifier of the vertex buffer in the device state.
         * \param vertex_buffer A \ref std::unique_ptr to the \ref IVertexBuffer to add.
         */
        void add(const std::string& id, std::unique_ptr<IVertexBuffer>&& vertex_buffer);

        /**
         * \brief Adds a \ref IIndexBuffer to the device state and uses its name as identifier.
         * \param index_buffer A \ref std::unique_ptr to the \ref IIndexBuffer to add.
         */
        void add(std::unique_ptr<IIndexBuffer>&& index_buffer);

        /**
         * \brief Adds a new \ref IIndexBuffer to the device state.
         * \param id The identifier of the index buffer in the device state.
         * \param index_buffer A \ref std::unique_ptr to the \ref IIndexBuffer to add.
         */
        void add(const std::string& id, std::unique_ptr<IIndexBuffer>&& index_buffer);

        /**
         * \brief Adds a \ref IImage to the device state and uses its name as identifier.
         * \param image A \ref std::unique_ptr to the \ref IImage to add.
         */
        void add(std::unique_ptr<IImage>&& image);

        /**
         * \brief Adds a new \ref IImage to the device state.
         * \param id The identifier of the image in the device state.
         * \param image A \ref std::unique_ptr to the \ref IImage to add.
         */
        void add(const std::string& id, std::unique_ptr<IImage>&& image);

        /**
         * \brief Adds a \ref ISampler to the device state and uses its name as identifier.
         * \param sampler A \ref std::unique_ptr to the \ref ISampler to add.
         */
        void add(std::unique_ptr<ISampler>&& sampler);

        /**
         * \brief Adds a new \ref ISampler to the device state.
         * \param id The identifier of the sampler in the device state.
         * \param sampler A \ref std::unique_ptr to the \ref ISampler to add.
         */
        void add(const std::string& id, std::unique_ptr<ISampler>&& sampler);

        /**
         * \brief Adds a \ref IDescriptorSet to the device state and uses its name as identifier.
         * \param id The identifier of the descriptor set in the device state.
         * \param descriptor_set A \ref std::unique_ptr to the \ref IDescriptorSet to add.
         */
        void add(const std::string& id, std::unique_ptr<IDescriptorSet>&& descriptor_set);

        /**
         * \brief Releases a \ref IRenderPass.
         * \param render_pass The \ref IRenderPass to release.
         * \return `true` if the render pass was successfully released, `false` otherwise.
         *
         * Calling this method will destroy the render pass. Before calling it, the render pass must be requested using \ref DeviceState::renderPass().
         * After this method has been executed, all references (including the \p renderPass parameter) will be invalid.
         * If the render pass is not managed by the device state, this method will do nothing and return <c>false</c>.
         */
        bool release(const IRenderPass& render_pass);

        /**
         * \brief Releases a \ref IPipeline.
         * \param pipeline The \ref IPipeline to release.
         * \return `true` if the pipeline was successfully released, `false` otherwise.
         */
        bool release(const IPipeline& pipeline);

        /**
         * \brief Releases a \ref IBuffer.
         * \param buffer The \ref IBuffer to release.
         * \return `true` if the buffer was successfully released, `false` otherwise.
         */
        bool release(const IBuffer& buffer);

        /**
         * \brief Releases a \ref IVertexBuffer.
         * \param vertex_buffer The \ref IVertexBuffer to release.
         * \return `true` if the vertex buffer was successfully released, `false` otherwise.
         */
        bool release(const IVertexBuffer& vertex_buffer);

        /**
         * \brief Releases a \ref IIndexBuffer.
         * \param index_buffer The \ref IIndexBuffer to release.
         * \return `true` if the index buffer was successfully released, `false` otherwise.
         */
        bool release(const IIndexBuffer& index_buffer);

        /**
         * \brief Releases a \ref IImage.
         * \param image The \ref IImage to release.
         * \return `true` if the image was successfully released, `false` otherwise.
         */
        bool release(const IImage& image);

        /**
         * \brief Releases a \ref ISampler.
         * \param sampler The \ref ISampler to release.
         * \return `true` if the sampler was successfully released, `false` otherwise.
         */
        bool release(const ISampler& sampler);

        /**
         * \brief Releases a \ref IDescriptorSet.
         * \param descriptor_set The \ref IDescriptorSet to release.
         * \return `true` if the descriptor set was successfully released, `false` otherwise.
         */
        bool release(const IDescriptorSet& descriptor_set);

        /**
         * \brief Releases all resources managed by the device state.
         */
        void clear();

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
