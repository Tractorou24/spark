#include "spark/render/DeviceState.h"

#include "spark/base/Exception.h"

namespace spark::render
{
    struct DeviceState::Impl
    {
        friend DeviceState;

    public:
        explicit Impl() = default;

    private:
        std::unordered_map<std::string, std::unique_ptr<IRenderPass>> m_renderPasses;
        std::unordered_map<std::string, std::unique_ptr<IPipeline>> m_pipelines;
        std::unordered_map<std::string, std::unique_ptr<IBuffer>> m_buffers;
        std::unordered_map<std::string, std::unique_ptr<IVertexBuffer>> m_vertexBuffers;
        std::unordered_map<std::string, std::unique_ptr<IIndexBuffer>> m_indexBuffers;
        std::unordered_map<std::string, std::unique_ptr<IImage>> m_images;
        std::unordered_map<std::string, std::unique_ptr<ISampler>> m_samplers;
        std::unordered_map<std::string, std::unique_ptr<IDescriptorSet>> m_descriptorSets;
    };

    DeviceState::DeviceState()
        : m_impl(std::make_unique<Impl>()) {}

    DeviceState::~DeviceState() = default;

    IRenderPass& DeviceState::renderPass(const std::string& id) const
    {
        if (!m_impl->m_renderPasses.contains(id))
            throw spark::base::BadArgumentException(std::format("No render pass with the identifier \"{0}\" has been registered in the device state.", id));

        return *m_impl->m_renderPasses.at(id);
    }

    IPipeline& DeviceState::pipeline(const std::string& id) const
    {
        if (!m_impl->m_pipelines.contains(id))
            throw spark::base::BadArgumentException(std::format("No pipeline with the identifier \"{0}\" has been registered in the device state.", id));

        return *m_impl->m_pipelines.at(id);
    }

    IBuffer& DeviceState::buffer(const std::string& id) const
    {
        if (!m_impl->m_buffers.contains(id))
            throw spark::base::BadArgumentException(std::format("No buffer with the identifier \"{0}\" has been registered in the device state.", id));

        return *m_impl->m_buffers.at(id);
    }

    IVertexBuffer& DeviceState::vertexBuffer(const std::string& id) const
    {
        if (!m_impl->m_vertexBuffers.contains(id))
            throw spark::base::BadArgumentException(std::format("No vertex buffer with the identifier \"{0}\" has been registered in the device state.", id));

        return *m_impl->m_vertexBuffers.at(id);
    }

    IIndexBuffer& DeviceState::indexBuffer(const std::string& id) const
    {
        if (!m_impl->m_indexBuffers.contains(id))
            throw spark::base::BadArgumentException(std::format("No index buffer with the identifier \"{0}\" has been registered in the device state.", id));

        return *m_impl->m_indexBuffers.at(id);
    }

    IImage& DeviceState::image(const std::string& id) const
    {
        if (!m_impl->m_images.contains(id))
            throw spark::base::BadArgumentException(std::format("No image with the identifier \"{0}\" has been registered in the device state.", id));

        return *m_impl->m_images.at(id);
    }

    ISampler& DeviceState::sampler(const std::string& id) const
    {
        if (!m_impl->m_samplers.contains(id))
            throw spark::base::BadArgumentException(std::format("No sampler with the identifier \"{0}\" has been registered in the device state.", id));

        return *m_impl->m_samplers.at(id);
    }

    IDescriptorSet& DeviceState::descriptorSet(const std::string& id) const
    {
        if (!m_impl->m_descriptorSets.contains(id))
            throw spark::base::BadArgumentException(std::format("No descriptor set with the identifier \"{0}\" has been registered in the device state.", id));

        return *m_impl->m_descriptorSets.at(id);
    }

    void DeviceState::add(std::unique_ptr<IRenderPass>&& render_pass)
    {
        add(render_pass->name(), std::move(render_pass));
    }

    void DeviceState::add(const std::string& id, std::unique_ptr<IRenderPass>&& render_pass)
    {
        if (!render_pass)
            throw spark::base::BadArgumentException("The render pass to add to the device state must be initialized.");

        if (m_impl->m_renderPasses.contains(id))
            throw spark::base::BadArgumentException(std::format("A render pass with the identifier \"{0}\" has already been registered in the device state.", id));

        m_impl->m_renderPasses.emplace(id, std::move(render_pass));
    }

    void DeviceState::add(std::unique_ptr<IPipeline>&& pipeline)
    {
        add(pipeline->name(), std::move(pipeline));
    }

    void DeviceState::add(const std::string& id, std::unique_ptr<IPipeline>&& pipeline)
    {
        if (!pipeline)
            throw spark::base::BadArgumentException("The pipeline to add to the device state must be initialized.");

        if (m_impl->m_pipelines.contains(id))
            throw spark::base::BadArgumentException(std::format("A pipeline with the identifier \"{0}\" has already been registered in the device state.", id));

        m_impl->m_pipelines.emplace(id, std::move(pipeline));
    }

    void DeviceState::add(std::unique_ptr<IBuffer>&& buffer)
    {
        add(buffer->name(), std::move(buffer));
    }

    void DeviceState::add(const std::string& id, std::unique_ptr<IBuffer>&& buffer)
    {
        if (!buffer)
            throw spark::base::BadArgumentException("The buffer to add to the device state must be initialized.");

        if (m_impl->m_buffers.contains(id))
            throw spark::base::BadArgumentException(std::format("A buffer with the identifier \"{0}\" has already been registered in the device state.", id));

        m_impl->m_buffers.emplace(id, std::move(buffer));
    }

    void DeviceState::add(std::unique_ptr<IVertexBuffer>&& vertex_buffer)
    {
        add(vertex_buffer->name(), std::move(vertex_buffer));
    }

    void DeviceState::add(const std::string& id, std::unique_ptr<IVertexBuffer>&& vertex_buffer)
    {
        if (!vertex_buffer)
            throw spark::base::BadArgumentException("The vertex buffer to add to the device state must be initialized.");

        if (m_impl->m_vertexBuffers.contains(id))
            throw spark::base::BadArgumentException(std::format("A vertex buffer with the identifier \"{0}\" has already been registered in the device state.", id));

        m_impl->m_vertexBuffers.emplace(id, std::move(vertex_buffer));
    }

    void DeviceState::add(std::unique_ptr<IIndexBuffer>&& index_buffer)
    {
        add(index_buffer->name(), std::move(index_buffer));
    }

    void DeviceState::add(const std::string& id, std::unique_ptr<IIndexBuffer>&& index_buffer)
    {
        if (!index_buffer)
            throw spark::base::BadArgumentException("The index buffer to add to the device state must be initialized.");

        if (m_impl->m_indexBuffers.contains(id))
            throw spark::base::BadArgumentException(std::format("An index buffer with the identifier \"{0}\" has already been registered in the device state.", id));

        m_impl->m_indexBuffers.emplace(id, std::move(index_buffer));
    }

    void DeviceState::add(std::unique_ptr<IImage>&& image)
    {
        add(image->name(), std::move(image));
    }

    void DeviceState::add(const std::string& id, std::unique_ptr<IImage>&& image)
    {
        if (!image)
            throw spark::base::BadArgumentException("The image to add to the device state must be initialized.");

        if (m_impl->m_images.contains(id))
            throw spark::base::BadArgumentException(std::format("An image with the identifier \"{0}\" has already been registered in the device state.", id));

        m_impl->m_images.emplace(id, std::move(image));
    }

    void DeviceState::add(std::unique_ptr<ISampler>&& sampler)
    {
        add(sampler->name(), std::move(sampler));
    }

    void DeviceState::add(const std::string& id, std::unique_ptr<ISampler>&& sampler)
    {
        if (!sampler)
            throw spark::base::BadArgumentException("The sampler to add to the device state must be initialized.");

        if (m_impl->m_samplers.contains(id))
            throw spark::base::BadArgumentException(std::format("A sampler with the identifier \"{0}\" has already been registered in the device state.", id));

        m_impl->m_samplers.emplace(id, std::move(sampler));
    }

    void DeviceState::add(const std::string& id, std::unique_ptr<IDescriptorSet>&& descriptor_set)
    {
        if (!descriptor_set)
            throw spark::base::BadArgumentException("The descriptor set to add to the device state must be initialized.");

        if (m_impl->m_descriptorSets.contains(id))
            throw spark::base::BadArgumentException(std::format("A descriptor set with the identifier \"{0}\" has already been registered in the device state.", id));

        m_impl->m_descriptorSets.emplace(id, std::move(descriptor_set));
    }

    bool DeviceState::release(const IRenderPass& render_pass)
    {
        const auto match = std::ranges::find_if(m_impl->m_renderPasses, [&render_pass](const auto& pair) { return pair.second.get() == &render_pass; });
        if (match == m_impl->m_renderPasses.end())
            return false;

        m_impl->m_renderPasses.erase(match);
        return true;
    }

    bool DeviceState::release(const IPipeline& pipeline)
    {
        const auto match = std::ranges::find_if(m_impl->m_pipelines, [&pipeline](const auto& pair) { return pair.second.get() == &pipeline; });
        if (match == m_impl->m_pipelines.end())
            return false;

        m_impl->m_pipelines.erase(match);
        return true;
    }

    bool DeviceState::release(const IBuffer& buffer)
    {
        const auto match = std::ranges::find_if(m_impl->m_buffers, [&buffer](const auto& pair) { return pair.second.get() == &buffer; });
        if (match == m_impl->m_buffers.end())
            return false;

        m_impl->m_buffers.erase(match);
        return true;
    }

    bool DeviceState::release(const IVertexBuffer& vertex_buffer)
    {
        const auto match = std::ranges::find_if(m_impl->m_vertexBuffers, [&vertex_buffer](const auto& pair) { return pair.second.get() == &vertex_buffer; });
        if (match == m_impl->m_vertexBuffers.end())
            return false;

        m_impl->m_vertexBuffers.erase(match);
        return true;
    }

    bool DeviceState::release(const IIndexBuffer& index_buffer)
    {
        const auto match = std::ranges::find_if(m_impl->m_indexBuffers, [&index_buffer](const auto& pair) { return pair.second.get() == &index_buffer; });
        if (match == m_impl->m_indexBuffers.end())
            return false;

        m_impl->m_indexBuffers.erase(match);
        return true;
    }

    bool DeviceState::release(const IImage& image)
    {
        const auto match = std::ranges::find_if(m_impl->m_images, [&image](const auto& pair) { return pair.second.get() == &image; });
        if (match == m_impl->m_images.end())
            return false;

        m_impl->m_images.erase(match);
        return true;
    }

    bool DeviceState::release(const ISampler& sampler)
    {
        const auto match = std::ranges::find_if(m_impl->m_samplers, [&sampler](const auto& pair) { return pair.second.get() == &sampler; });
        if (match == m_impl->m_samplers.end())
            return false;

        m_impl->m_samplers.erase(match);
        return true;
    }

    bool DeviceState::release(const IDescriptorSet& descriptor_set)
    {
        const auto match = std::ranges::find_if(m_impl->m_descriptorSets, [&descriptor_set](const auto& pair) { return pair.second.get() == &descriptor_set; });
        if (match == m_impl->m_descriptorSets.end())
            return false;

        m_impl->m_descriptorSets.erase(match);
        return true;
    }

    void DeviceState::clear()
    {
        std::ranges::for_each(m_impl->m_descriptorSets, [](auto& pair) { pair.second = nullptr; });
        m_impl->m_descriptorSets.clear();
        std::ranges::for_each(m_impl->m_buffers, [](auto& pair) { pair.second = nullptr; });
        m_impl->m_buffers.clear();
        std::ranges::for_each(m_impl->m_vertexBuffers, [](auto& pair) { pair.second = nullptr; });
        m_impl->m_vertexBuffers.clear();
        std::ranges::for_each(m_impl->m_indexBuffers, [](auto& pair) { pair.second = nullptr; });
        m_impl->m_indexBuffers.clear();
        std::ranges::for_each(m_impl->m_images, [](auto& pair) { pair.second = nullptr; });
        m_impl->m_images.clear();
        std::ranges::for_each(m_impl->m_samplers, [](auto& pair) { pair.second = nullptr; });
        m_impl->m_samplers.clear();
        std::ranges::for_each(m_impl->m_pipelines, [](auto& pair) { pair.second = nullptr; });
        m_impl->m_pipelines.clear();
        std::ranges::for_each(m_impl->m_renderPasses, [](auto& pair) { pair.second = nullptr; });
        m_impl->m_renderPasses.clear();
    }
}
