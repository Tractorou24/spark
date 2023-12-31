#pragma once

#include "spark/render/Export.h"
#include "spark/render/PushConstantsRange.h"

#include <algorithm>
#include <vector>

namespace spark::render
{
    /**
     * \brief Interface for a push constants layout.
     */
    class SPARK_RENDER_EXPORT IPushConstantsLayout
    {
    public:
        virtual ~IPushConstantsLayout() noexcept = default;

        /**
         * \brief Get the size (in bytes) of the push constants backing memory.
         * \return The size (in bytes) of the push constants backing memory.
         */
        [[nodiscard]] virtual unsigned int size() const noexcept = 0;

        /**
         * \brief Get the \link IPushConstantsRange push constants range \endlink associated with the shader \p stage provided.
         * \return The \link IPushConstantsRange push constants range \endlink associated with the shader \p stage provided.
         *
         * \throws spark::base::ArgumentOutOfRangeException if no range is mapped to the provided shader stage.
         * \throws spark::base::BadArgumentException if \p stage contains multiple shader stages.
         */
        [[nodiscard]] virtual const IPushConstantsRange& range(ShaderStage stage) const = 0;

        /**
         * \brief Get all \link IPushConstantsRange push constants ranges \endlink.
         * \return All \link IPushConstantsRange push constants ranges \endlink.
         */
        [[nodiscard]] std::vector<const IPushConstantsRange*> ranges() const noexcept { return genericRanges(); }

    private:
        /// \brief Private method used to allow replacement of the generic methods by a custom \ref IPushConstantsRange type.
        [[nodiscard]] virtual std::vector<const IPushConstantsRange*> genericRanges() const noexcept = 0;
    };

    /**
     * \brief Describes the layout of the pipelines push constant ranges.
     * \tparam PushConstantsRangeType The type of the push constant ranges (inherits from \ref IPushConstantsRange).
     *
     * Push constants are very efficient, yet quite limited ways of passing per-draw data to shaders. They are mapped directly, so no buffer must be created or transitioned in order to 
     * use them. Conceptually a push constant is a single piece of memory that gets transferred to a command buffer directly. Each shader stage has a view into this piece of memory, 
     * described by an offset and a size. Since the memory is directly dumped in the command buffer, the backing memory can be incrementally updated and there is no need to store an
     * array of buffers, as long as updates happen sequentially from the same thread.
     *
     * However, there are certain restrictions when using push constants:
     *     - Only one push constant layout per pipeline layout is supported.
     *     - A push constant layout may not define a overall memory size larger than 128 bytes. This is a soft restriction that will issue a warning, however it might be supported on some hardware or for some back-ends.
     *     - The elements and offsets of a push constant memory block must be aligned to 4 bytes.
     *     - Accordingly, the size of a push constant layout must align to 4 bytes.
     *     - Only one \ref IPushConstantsRange per shader stage is permitted. Shader stages can be combined together, however in this case, no other ranges must be defined for the stages.
     *
     * Push constants can be updated by calling \ref ICommandBuffer::pushConstants() and are visible to subsequent draw calls immediately, until another update is performed or the command buffer is ended.
    */
    template <typename PushConstantsRangeType>
    class PushConstantsLayout : public IPushConstantsLayout
    {
    public:
        using PushConstantsRange = PushConstantsRangeType;

    public:
        /// \copydoc IPushConstantsLayout::range()
        [[nodiscard]] const PushConstantsRangeType& range(ShaderStage stage) const override = 0;

        /// \copydoc IPushConstantsLayout::ranges()
        [[nodiscard]] virtual std::vector<const PushConstantsRangeType*> ranges() const noexcept = 0;

    private:
        [[nodiscard]] virtual std::vector<const IPushConstantsRange*> genericRanges() const noexcept final
        {
            std::vector<const IPushConstantsRange*> ranges_vector;
            ranges_vector.reserve(ranges().size());
            std::ranges::transform(ranges(), std::back_inserter(ranges_vector), [](const auto& range) { return static_cast<const IPushConstantsRange*>(range); });
            return ranges_vector;
        }
    };
}
