#pragma once

#include "spark/render/Buffer.h"
#include "spark/render/DescriptorBinding.h"
#include "spark/render/Export.h"
#include "spark/render/Sampler.h"
#include "spark/render/ShaderStages.h"

#include <algorithm>
#include <memory>

namespace spark::render
{
    /**
     * \brief Describes the type of a \ref IDescriptor.
     * \note In theory, we can declare a writable descriptor in any format, the rendering back-end might not necessarily support writing in a specific format.
     */
    enum class DescriptorType
    {
        // ReSharper disable CppInconsistentNaming

        /// \brief A read-only uniform buffer in Vulkan. In GLSL, use the `uniform` keyword to access a uniform buffer. In HLSL, use the `ConstantBuffer` keyword.
        ConstantBuffer = 0x00000001,

        /// \brief A shader storage buffer object in Vulkan. In GLSL, use the `buffer` keyword to access storage buffers. In HLSL, use the `StructuredBuffer` keyword.
        /// A storage buffer is read-only by default. If you want to create a writable storage buffer, use `WritableStorage` instead.
        /// The difference between \c ConstantBuffer and storage buffers is, that storage buffers can have variable length. However, they are typically less efficient.
        StructuredBuffer = 0x00000002,

        /// \brief A writable shader storage object in Vulkan. In GLSL, use the `buffer` keyword to access storage buffers. In HLSL, use the `RWStructuredBuffer` keyword.
        RWStructuredBuffer = 0x00000012,

        /// \brief A read-only sampled image. In GLSL, use the `uniform texture` keywords to access the texture. In HLSL, use the `Texture` keywords.
        /// Textures are read-only by default. If you want to create a writable texture, use the `WritableTexture` instead.
        /// Note, that textures are typically not be accessed directly, but instead are sampled using a \ref ISampler.
        Texture = 0x00000003,

        /// \brief A writable image. In GLSL, use the `uniform image` keywords to access the texture. In HLSL, use the `RWTexture` keywords.
        RWTexture = 0x00000013,

        /// \brief A sampler state of a texture or image.
        Sampler = 0x00000004,

        /// \brief The result of a render target from an earlier render pass. Maps to a `SubpassInput` in HLSL.
        InputAttachment = 0x00000005,

        /// \brief Represents a read-only texel buffer (uniform texel buffer). Use the `uniform imageBuffer` keyword in GLSL to access the buffer. In HLSL, use the `Buffer` keyword.
        Buffer = 0x00000006,

        /// \brief Represents a writable texel buffer (storage texel buffer). Use the `uniform imageBuffer` keyword in GLSL to access the buffer. In HLSL, use the `RWBuffer` keyword.
        RWBuffer = 0x00000016,

        /// \brief Represents an unformatted buffer. In GLSL, use the `buffer` keyword to access byte address buffers. In HLSL, use the `ByteAddressBuffer` keyword.
        ByteAddressBuffer = 0x00000007,

        /// \brief Represents an unformatted writable buffer. In GLSL, use the `buffer` keyword to access byte address buffers. In HLSL, use the `RWByteAddressBuffer` keyword.
        RWByteAddressBuffer = 0x00000017,

        // ReSharper restore CppInconsistentNaming
    };

    /**
     * \brief Describes a the layout of a single descriptor within a \ref DescriptorSet.
     *
     * A common metaphor for a descriptor to think of it as a "pointer for the GPU". Basically, a descriptor points to a buffer in a shader. A descriptor 
     * can have different types and sizes. The types a descriptor can have are described by the \ref DescriptorType.
     * 
     * If the descriptor is a sampler, it can either be a dynamic or static sampler. A dynamic sampler needs to be bound during runtime just like any other
     * descriptor by calling \ref IDescriptorSet::update(). A static sampler is defined alongside the descriptor layout and is automatically set
     * when the pipeline that uses the descriptor layout gets bound. In this case, the descriptor must not be updated with another sampler. If a descriptor
     * layout describes a static sampler, the \ref IDescriptorLayout::staticSampler() returns a pointer to the static sampler state.
     */
    class SPARK_RENDER_EXPORT IDescriptorLayout : public IBufferLayout
    {
    public:
        ~IDescriptorLayout() noexcept override = default;

        /**
         * \brief Gets the type of the descriptor.
         * \return A \ref DescriptorType representing the type of the descriptor.
         */
        [[nodiscard]] virtual DescriptorType descriptorType() const noexcept = 0;

        /**
         * \brief Gets the number of descriptors in the descriptor array, or `0xFFFFFFFF` if the descriptor is unbounded.
         * \return The number of descriptors in the descriptor array, or `0xFFFFFFFF` if the descriptor is unbounded.
         *
         * \note If the descriptor is unbounded, the size of the array must be specified when allocating a \ref IDescriptorSet using the `descriptors` parameter.
         */
        [[nodiscard]] virtual unsigned int descriptors() const noexcept = 0;

        /**
         * \brief Gets the state of the sampler if it is a static sampler.
         * \return A pointer to \ref ISampler containing the state of the sampler if the descriptor is a static sampler, or `nullptr` if the descriptor is not a static sampler.
         */
        [[nodiscard]] virtual const ISampler* staticSampler() const noexcept = 0;
    };

    /**
     * \brief Interface for a descriptor set.
     */
    class SPARK_RENDER_EXPORT IDescriptorSet
    {
    public:
        virtual ~IDescriptorSet() noexcept = default;

        /**
         * \brief Updates a constant buffer within the \ref IDescriptorSet.
         * \param binding The buffer binding point.
         * \param buffer The constant buffer to write to the \ref IDescriptorSet.
         * \param buffer_element The index of the first element to write to the \ref IDescriptorSet.
         * \param elements The number of elements to write to the \ref IDescriptorSet, or `0` to write all elements starting from \p buffer_element.
         * \param first_descriptor The index of the first descriptor in the descriptor array to update.
         */
        void update(unsigned int binding, const IBuffer& buffer, unsigned int buffer_element = 0, unsigned int elements = 0, unsigned int first_descriptor = 0) const
        {
            genericUpdate(binding, buffer, buffer_element, elements, first_descriptor);
        }

        /**
         * \brief Updates a texture within the current descriptor set.
         * \param binding The binding point of the texture.
         * \param texture The texture to write to the descriptor set.
         * \param descriptor The index of the descriptor in the descriptor array to bind the texture to.
         * \param first_level The index of the first mip-map level to bind.
         * \param levels The number of mip-map levels to bind. A value of `0` binds all available levels, starting at \p firstLevel.
         * \param first_layer The index of the first layer to bind.
         * \param layers The number of layers to bind. A value of `0` binds all available layers, starting at \p firstLayer.
         */
        void update(unsigned int binding,
                    const IImage& texture,
                    unsigned int descriptor = 0,
                    unsigned int first_level = 0,
                    unsigned int levels = 0,
                    unsigned int first_layer = 0,
                    unsigned int layers = 0) const { genericUpdate(binding, texture, descriptor, first_level, levels, first_layer, layers); }

        /**
         * \brief Updates a sampler within the current descriptor set.
         * \param binding The binding point of the sampler.
         * \param sampler The sampler to write to the descriptor set.
         * \param descriptor The index of the descriptor in the descriptor array to bind the sampler to.
         */
        void update(unsigned int binding, const ISampler& sampler, unsigned int descriptor = 0) const { genericUpdate(binding, sampler, descriptor); }

        /**
         * \brief Attaches an image as an input attachment to a descriptor bound at \p binding.
         * \param binding The input attachment binding point.
         * \param image The image to bind to the input attachment descriptor.
         */
        void attach(unsigned int binding, const IImage& image) const { genericAttach(binding, image); }

    private:
        /// @{
        /// \brief Private method used to allow the replacement of the generic methods by a custom \ref IBuffer type.
        virtual void genericUpdate(unsigned int binding, const IBuffer& buffer, unsigned int buffer_element, unsigned int elements, unsigned int first_descriptor) const = 0;
        virtual void genericUpdate(unsigned int binding,
                                   const IImage& texture,
                                   unsigned int descriptor,
                                   unsigned int first_level,
                                   unsigned int levels,
                                   unsigned int first_layer,
                                   unsigned int layers) const = 0;
        virtual void genericUpdate(unsigned int binding, const ISampler& sampler, unsigned int descriptor) const = 0;
        virtual void genericAttach(unsigned int binding, const IImage& image) const = 0;
        /// @}
    };

    /**
     * \brief Defines a set of descriptors.
     * \tparam BufferType The type of the buffer interface. (inherits from \ref IBuffer)
     * \tparam ImageType The type of the image interface. (inherits from \ref IImage)
     * \tparam SamplerType The type of the sampler interface. (inherits from \ref ISampler)
     *
     * Descriptors can be grouped into multiple descriptor sets. It is generally a good practice to group descriptors based on the frequency of the updates they receive.
     * For example, it typically makes sense to store the camera buffer in a descriptor set, since it only needs to be updated once per frame for each camera, whilst the object
     * or material data should be stored in separate descriptor sets, that are possibly updated before each draw call.
     *
     * From a shader perspective, a descriptor set is identified by a `set` (GLSL) or `space` (HLSL), whilst a descriptor is addressed by a `binding` (GLSL) or 
     * `register` (HLSL). Descriptor sets are read from GPU-visible memory, depending on how they are bound during the current draw call.
     * 
     * From a CPU perspective, think of a descriptor set as an array of pointers to different buffers (i.e. descriptors) for the shader. A descriptor can be bound to a set by 
     * calling \ref IDescriptorSet::update(). Note that this does not automatically ensure, that the buffer memory is visible for the GPU. Instead, a buffer may also 
     * require a transfer into GPU visible memory, depending on the \ref BufferUsage. However, as long as a descriptor within a set is mapped to a buffer, modifying 
     * this buffer also reflects the change to the shader, without requiring to update the descriptor, similarly to how modifying the object behind a pointer does not require 
     * the pointer to change.
     * 
     * Note, that there might be multiple descriptor set instances of the same \ref DescriptorSetLayout, pointing to different \ref IBuffer instances, depending on the number
     * of frames in flight. Since multiple frames can be computed concurrently, it is important to properly synchronize descriptor set updates.
     *
     * Generally, there are three strategies to choose from, that you can implement or mix in custom flavors, depending on your use case:
     *     - Naive: The naive approach most closely matches earlier graphics API concepts. Create one buffer per descriptor and synchronize frames. This basically means that each 
     *         back buffer swap is synchronized to wait for the graphics pipeline. This way, writing to a buffer ensures, that it is only read within the frame of reference and 
     *         modifying it does not interfere with other frames. This strategy is memory efficient, but may cause the GPU to stall. It may, however be a valid strategy, for 
     *         data that is only written once or very infrequently.
     *     - Array of Buffers: The helper methods for creating and updating constant buffers are able to create buffer arrays. Those arrays can be used to create a buffer for each frame in 
     *         flight. When binding a buffer to a descriptor, it is possible to bind only one element of the array. This way, each frame has its own buffer and does not 
     *         interfere with other buffer writes.
     *     - Ring-Buffer: The most efficient (yet not always applicable) approach involves creating one large buffer array, that is bound to multiple descriptor sets. This ensures that
     *         the buffer memory stays contiguous and does not get fragmented. However, this requires to know upfront, how many buffers are required for each descriptor, which
     *         might not always be possible. Thus another flavor of using this technique involves a creating a large enough descriptor array and updating the descriptor
     *         set with an increasing array element for each object as a ring-buffer. As long as there are enough elements in the buffer, so that no second update interferes 
     *         with a buffer write in an earlier frame, this method provides the most efficient approach. However, it may be hard or impossible to determine the ideal size of 
     *         the ring-buffer upfront.
     * 
     * Note that samplers, textures and input attachments currently do not support array binding, since they are typically only updated once or require pipeline 
     * synchronization anyway.
     */
    template <typename BufferType, typename ImageType, typename SamplerType>
    class DescriptorSet : public IDescriptorSet
    {
    public:
        using buffer_type = BufferType;
        using image_type = ImageType;
        using sampler_type = SamplerType;

    public:
        /// \copydoc IDescriptorSet::update()
        virtual void update(unsigned int binding,
                            const buffer_type& buffer,
                            unsigned int buffer_element = 0,
                            unsigned int elements = 0,
                            unsigned int first_descriptor = 0) const = 0;

        /// \copydoc IDescriptorSet::update()
        virtual void update(unsigned int binding,
                            const image_type& texture,
                            unsigned int descriptor = 0,
                            unsigned int first_level = 0,
                            unsigned int levels = 0,
                            unsigned int first_layer = 0,
                            unsigned int layers = 0) const = 0;

        /// \copydoc IDescriptorSet::update()
        virtual void update(unsigned int binding, const sampler_type& sampler, unsigned int descriptor = 0) const = 0;

        /// \copydoc IDescriptorSet::attach()
        virtual void attach(unsigned int binding, const image_type& image) const = 0;

    private:
        void genericUpdate(unsigned binding, const IBuffer& buffer, unsigned buffer_element, unsigned elements, unsigned first_descriptor) const final
        {
            update(binding, dynamic_cast<const buffer_type&>(buffer), buffer_element, elements, first_descriptor);
        }

        void genericUpdate(unsigned binding,
                           const IImage& texture,
                           unsigned descriptor,
                           unsigned first_level,
                           unsigned levels,
                           unsigned first_layer,
                           unsigned layers) const override { update(binding, dynamic_cast<const image_type&>(texture), descriptor, first_level, levels, first_layer, layers); }

        void genericUpdate(unsigned binding, const ISampler& sampler, unsigned descriptor) const final { update(binding, dynamic_cast<const sampler_type&>(sampler), descriptor); }
        void genericAttach(unsigned binding, const IImage& image) const final { attach(binding, dynamic_cast<const image_type&>(image)); }
    };

    /**
     * \brief Interface for a descriptor set layout.
     */
    class SPARK_RENDER_EXPORT IDescriptorSetLayout
    {
    public:
        virtual ~IDescriptorSetLayout() noexcept = default;

        /**
         * \brief Gets the layouts of all descriptors in the \ref IDescriptorSet.
         * \return A \ref std::vector containing pointers to \ref IDescriptorLayout of all descriptors in the \ref IDescriptorSet.
         */
        [[nodiscard]] std::vector<const IDescriptorLayout*> descriptors() const noexcept { return genericDescriptors(); }

        /**
         * \brief Gets the layout of a descriptor in the \ref IDescriptorSet bound to a specific binding point.
         * \param binding The binding point of the requested descriptor layout.
         * \return A pointer to the \ref IDescriptorLayout describing the layout of the descriptor bound to \p binding.
         */
        [[nodiscard]] virtual const IDescriptorLayout* descriptor(unsigned int binding) const = 0;

        /**
         * \brief Gets the space index of the \ref IDescriptorSet.
         * \return The space index of the \ref IDescriptorSet.
         *
         * \note This maps the space index in HLSL.
         */
        [[nodiscard]] virtual unsigned int space() const noexcept = 0;

        /**
         * \brief Gets the shader stage the \ref IDescriptorSet is used in.
         * \return A \ref ShaderStage representing the shader stage the \ref IDescriptorSet is used in.
         */
        [[nodiscard]] virtual ShaderStage shaderStage() const noexcept = 0;

        /**
         * \brief Gets the number of uniform/constant buffers in the \ref IDescriptorSet.
         * \return The number of uniform/constant buffers in the \ref IDescriptorSet.
         */
        [[nodiscard]] virtual unsigned int uniforms() const noexcept = 0;

        /**
         * \brief Gets the number of storage buffers in the \ref IDescriptorSet.
         * \return The number of storage buffers in the \ref IDescriptorSet.
         */
        [[nodiscard]] virtual unsigned int storages() const noexcept = 0;

        /**
         * \brief Gets the number of images (textures) in the \ref IDescriptorSet.
         * \return The number of images (textures) in the \ref IDescriptorSet.
         */
        [[nodiscard]] virtual unsigned int images() const noexcept = 0;

        /**
         * \brief Get the number of texel buffer descriptors in the \ref IDescriptorSet.
         * \return The number of texel buffer descriptors in the \ref IDescriptorSet.
         */
        [[nodiscard]] virtual unsigned int buffers() const noexcept = 0;

        /**
         * \brief Gets the number of texel buffer descriptors in the \ref IDescriptorSet.
         * \return The number of texel buffer descriptors in the \ref IDescriptorSet.
         */
        [[nodiscard]] virtual unsigned int samplers() const noexcept = 0;

        /**
         * \brief Gets the number of static/immutable samplers in the \ref IDescriptorSet.
         * \return The number of static/immutable samplers in the \ref IDescriptorSet.
         */
        [[nodiscard]] virtual unsigned int staticSamplers() const noexcept = 0;

        /**
         * \brief Gets the number of input attachments in the \ref IDescriptorSet.
         * \return The number of input attachments in the \ref IDescriptorSet.
         */
        [[nodiscard]] virtual unsigned int inputAttachments() const noexcept = 0;

        /**
         * \brief Allocates a new \ref IDescriptorSet or returns an unused descriptor set.
         * \param bindings Optional list of descriptor bindings to initialize the \ref IDescriptorSet with.
         * \return A \ref std::unique_ptr to the allocated \ref IDescriptorSet.
         *
         * Allocating a new descriptor set may be an expensive operation. To improve performance, and prevent fragmentation, the descriptor set layout keeps track of
         * created descriptor sets. It does this by never releasing them. Instead, when a \ref IDescriptorSet instance gets destroyed, it should call 
         * \ref IDescriptorSetLayout::free() in order to mark itself (i.e. its handle) as not being used any longer.
         * 
         * Before allocating a new descriptor set from a pool (which may even result in the creation of a new pool, if the existing pools are full), the layout tries 
         * to hand out descriptor sets that marked as unused. Descriptor sets are only deleted, if the whole layout instance and therefore the descriptor pools are 
         * deleted.
         * (
         * The above does not apply to unbounded descriptor arrays. A unbounded descriptor array is one, for which \ref IDescriptorLayout::descriptors() 
         * returns `-1` (or `0xFFFFFFFF`). They must be allocated by specifying the \p descriptors parameter. This parameter defines the number of
         * descriptors to allocate in the array. 
         * 
         * Note that descriptor sets, that contain an unbounded descriptor array must only contain one single descriptor (the one that identifies this array). Such 
         * descriptor sets are never cached. Instead, they are released when calling \ref IDescriptorSetLayout::free(). It is a good practice to cache such descriptor
         * sets as global descriptor tables once and never release them. They provide more flexibility than regular descriptor arrays, since they may be updated, even after
         * they have been bound to a command buffer or from different threads. However, you must ensure yourself not to overwrite any descriptors that are currently
         * in use. Because unbounded arrays are not cached, freeing and re-allocating such descriptor sets may leave the descriptor heap fragmented, which might cause
         * the allocation to fail, if the heap is full.
         */
        [[nodiscard]] std::unique_ptr<IDescriptorSet> allocate(const std::vector<DescriptorBinding>& bindings = {}) const { return genericAllocate(0, bindings); }

        /**
         * \brief Allocates a new \ref IDescriptorSet or returns an unused descriptor set.
         * \param descriptors The number of descriptors to allocate in an unbounded descriptor array. Ignored if the descriptor array is bounded.
         * \param bindings Optional list of descriptor bindings to initialize the \ref IDescriptorSet with.
         * \return A \ref std::unique_ptr to the allocated \ref IDescriptorSet.
         */
        [[nodiscard]] std::unique_ptr<IDescriptorSet> allocate(unsigned int descriptors, const std::vector<DescriptorBinding>& bindings = {}) const
        {
            return genericAllocate(descriptors, bindings);
        }

        /**
         * \brief Allocates an array of \ref IDescriptorSet.
         * \param descriptors_sets The number of \ref IDescriptorSet to allocate.
         * \param bindings Optional list of descriptor bindings to initialize the \ref IDescriptorSet with.
         * \return A \ref std::vector containing \ref std::unique_ptr to the newly allocated \link IDescriptorSet descriptor sets \endlink .
         */
        [[nodiscard]] std::vector<std::unique_ptr<IDescriptorSet>> allocateMultiple(unsigned int descriptors_sets,
                                                                                    const std::vector<std::vector<DescriptorBinding>>& bindings = {}) const noexcept
        {
            return allocateMultiple(descriptors_sets, 0, bindings);
        }

        /**
         * \brief Allocates an array of \ref IDescriptorSet.
         * \param descriptors_sets The number of \ref IDescriptorSet to allocate.
         * \param binding_factory A factory function that creates a list of descriptor bindings for each \ref IDescriptorSet.
         * \return A \ref std::vector containing \ref std::unique_ptr to the newly allocated \link IDescriptorSet descriptor sets \endlink .
         */
        [[nodiscard]] std::vector<std::unique_ptr<IDescriptorSet>> allocateMultiple(unsigned int descriptors_sets,
                                                                                    const std::function<std::vector<DescriptorBinding>(unsigned)>& binding_factory) const noexcept
        {
            return allocateMultiple(descriptors_sets, 0, binding_factory);
        }

        /**
         * \brief Allocates an array of \ref IDescriptorSet.
         * \param descriptors_sets The number of \ref IDescriptorSet to allocate.
         * \param descriptors The number of descriptors to allocate in an unbounded descriptor array. Ignored if the descriptor array is bounded.
         * \param bindings Optional list of descriptor bindings to initialize each \ref IDescriptorSet with.
         * \return A \ref std::vector containing \ref std::unique_ptr to the newly allocated \link IDescriptorSet descriptor sets \endlink .
         */
        [[nodiscard]] std::vector<std::unique_ptr<IDescriptorSet>> allocateMultiple(unsigned int descriptors_sets,
                                                                                    unsigned int descriptors,
                                                                                    const std::vector<std::vector<DescriptorBinding>>& bindings = {}) const noexcept
        {
            return genericAllocate(descriptors_sets, descriptors, bindings);
        }

        /**
         * \brief Allocates an array of \ref IDescriptorSet.
         * \param descriptors_sets The number of \ref IDescriptorSet to allocate.
         * \param descriptors The number of descriptors to allocate in an unbounded descriptor array. Ignored if the descriptor array is bounded.
         * \param binding_factory A factory function that creates a list of descriptor bindings for each \ref IDescriptorSet.
         * \return A \ref std::vector containing \ref std::unique_ptr to the newly allocated \link IDescriptorSet descriptor sets \endlink .
         */
        [[nodiscard]] std::vector<std::unique_ptr<IDescriptorSet>> allocateMultiple(unsigned int descriptors_sets,
                                                                                    unsigned int descriptors,
                                                                                    const std::function<std::vector<DescriptorBinding>(unsigned)>& binding_factory = {}) const
            noexcept { return genericAllocate(descriptors_sets, descriptors, binding_factory); }

        /**
         * \brief Marks a \ref IDescriptorSet as unused, so that it can be handed out again instead of allocating a new one.
         * \param descriptor_set The \ref IDescriptorSet to mark as unused.
         */
        void free(const IDescriptorSet& descriptor_set) const noexcept { genericFree(descriptor_set); }

    private:
        /// @{
        /// \brief Private method used to allow the replacement of the generic methods by a custom \ref IDescriptorLayout type.
        [[nodiscard]] virtual std::vector<const IDescriptorLayout*> genericDescriptors() const noexcept = 0;
        [[nodiscard]] virtual std::unique_ptr<IDescriptorSet> genericAllocate(unsigned int descriptors, const std::vector<DescriptorBinding>& bindings) const noexcept = 0;
        [[nodiscard]] virtual std::vector<std::unique_ptr<IDescriptorSet>> genericAllocate(unsigned descriptor_sets,
                                                                                           unsigned descriptors,
                                                                                           const std::vector<std::vector<DescriptorBinding>>& bindings) const = 0;
        [[nodiscard]] virtual std::vector<std::unique_ptr<IDescriptorSet>> genericAllocate(unsigned descriptor_sets,
                                                                                           unsigned descriptors,
                                                                                           const std::function<std::vector<DescriptorBinding>(unsigned)>& binding_factory) const =
        0;
        virtual void genericFree(const IDescriptorSet& descriptor_set) const noexcept = 0;
        /// @}
    };

    /**
     * \brief Describes the layout of a descriptor set.
     * \tparam DescriptorLayoutType Type of the descriptor layout. (inherits from \ref IDescriptorLayout)
     * \tparam DescriptorSetType Type of the descriptor set. (inherits from \ref IDescriptorSet)
     */
    template <typename DescriptorLayoutType, typename DescriptorSetType>
    class DescriptorSetLayout : public IDescriptorSetLayout
    {
    public:
        using descriptor_layout_type = DescriptorLayoutType;
        using descriptor_set_type = DescriptorSetType;

    public:
        /// \copydoc IDescriptorSetLayout::descriptors()
        [[nodiscard]] virtual std::vector<const descriptor_layout_type*> descriptors() const noexcept = 0;

        /// \copydoc IDescriptorSetLayout::descriptor()
        [[nodiscard]] const descriptor_layout_type* descriptor(unsigned binding) const override = 0;

        /// \copydoc IDescriptorSetLayout::allocate()
        [[nodiscard]] virtual std::unique_ptr<descriptor_set_type> allocate(const std::vector<DescriptorBinding>& bindings = {}) const = 0;

        /// \copydoc IDescriptorSetLayout::allocate()
        [[nodiscard]] virtual std::unique_ptr<descriptor_set_type> allocate(unsigned descriptors, const std::vector<DescriptorBinding>& bindings = { }) const = 0;

        /// \copydoc IDescriptorSetLayout::allocateMultiple()
        [[nodiscard]] virtual std::vector<std::unique_ptr<descriptor_set_type>> allocateMultiple(unsigned descriptor_sets, const std::vector<std::vector<DescriptorBinding>>& bindings = { }) const = 0;

        /// \copydoc IDescriptorSetLayout::allocateMultiple()
        [[nodiscard]] virtual std::vector<std::unique_ptr<descriptor_set_type>> allocateMultiple(unsigned descriptor_sets, const std::function<std::vector<DescriptorBinding>(unsigned)>& binding_factory) const = 0;

        /// \copydoc IDescriptorSetLayout::allocateMultiple()
        [[nodiscard]] virtual std::vector<std::unique_ptr<descriptor_set_type>> allocateMultiple(unsigned descriptor_sets, unsigned descriptors, const std::vector<std::vector<DescriptorBinding>>& bindings = { }) const = 0;

        /// \copydoc IDescriptorSetLayout::allocateMultiple()
        [[nodiscard]] virtual std::vector<std::unique_ptr<descriptor_set_type>> allocateMultiple(unsigned descriptor_sets, unsigned descriptors, const std::function<std::vector<DescriptorBinding>(unsigned)>& binding_factory) const = 0;

        /// \copydoc IDescriptorSetLayout::free()
        virtual void free(const descriptor_set_type& descriptor_set) const noexcept = 0;

    private:
        [[nodiscard]] std::vector<const IDescriptorLayout*> genericDescriptors() const noexcept final
        {
            auto tmp = descriptors();
            std::vector<const IDescriptorLayout*> result;
            result.reserve(tmp.size());
            std::ranges::transform(tmp, std::back_inserter(result), [](auto& ptr) { return static_cast<const IDescriptorLayout*>(ptr); });
            return result;
        }

        [[nodiscard]] std::unique_ptr<IDescriptorSet> genericAllocate(unsigned descriptors, const std::vector<DescriptorBinding>& bindings) const noexcept final
        {
            return allocate(descriptors, bindings);
        }

        [[nodiscard]] std::vector<std::unique_ptr<IDescriptorSet>> genericAllocate(unsigned descriptor_sets,
                                                                                   unsigned descriptors,
                                                                                   const std::vector<std::vector<DescriptorBinding>>& bindings) const final
        {
            auto tmp = allocateMultiple(descriptor_sets, descriptors, bindings);
            std::vector<std::unique_ptr<IDescriptorSet>> result;
            result.reserve(tmp.size());
            std::ranges::transform(tmp, std::back_inserter(result), [](auto& ptr) { return std::unique_ptr<IDescriptorSet>(ptr.release()); });
            return result;
        }

        [[nodiscard]] std::vector<std::unique_ptr<IDescriptorSet>> genericAllocate(unsigned descriptor_sets,
                                                                                   unsigned descriptors,
                                                                                   const std::function<std::vector<DescriptorBinding>(unsigned)>& binding_factory) const final
        {
            auto tmp = allocateMultiple(descriptor_sets, descriptors, binding_factory);
            std::vector<std::unique_ptr<IDescriptorSet>> result;
            result.reserve(tmp.size());
            std::ranges::transform(tmp, std::back_inserter(result), [](auto& ptr) { return std::unique_ptr<IDescriptorSet>(ptr.release()); });
            return result;
        }

        void genericFree(const IDescriptorSet& descriptor_set) const noexcept final { free(dynamic_cast<const descriptor_set_type&>(descriptor_set)); }
    };
}

template <>
struct std::formatter<spark::render::DescriptorType> : std::formatter<std::string_view>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const spark::render::DescriptorType type, auto& ctx)
    {
        switch (type)
        {
        case spark::render::DescriptorType::ConstantBuffer:
            return std::format_to(ctx.out(), "ConstantBuffer");
        case spark::render::DescriptorType::StructuredBuffer:
            return std::format_to(ctx.out(), "StructuredBuffer");
        case spark::render::DescriptorType::RWStructuredBuffer:
            return std::format_to(ctx.out(), "RWStructuredBuffer");
        case spark::render::DescriptorType::Texture:
            return std::format_to(ctx.out(), "Texture");
        case spark::render::DescriptorType::RWTexture:
            return std::format_to(ctx.out(), "RWTexture");
        case spark::render::DescriptorType::Sampler:
            return std::format_to(ctx.out(), "Sampler");
        case spark::render::DescriptorType::InputAttachment:
            return std::format_to(ctx.out(), "InputAttachment");
        case spark::render::DescriptorType::Buffer:
            return std::format_to(ctx.out(), "Buffer");
        case spark::render::DescriptorType::RWBuffer:
            return std::format_to(ctx.out(), "RWBuffer");
        case spark::render::DescriptorType::ByteAddressBuffer:
            return std::format_to(ctx.out(), "ByteAddressBuffer");
        case spark::render::DescriptorType::RWByteAddressBuffer:
            return std::format_to(ctx.out(), "RWByteAddressBuffer");
        }
        return std::format_to(ctx.out(), "Unknown");
    }
};
