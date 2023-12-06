#pragma once

#include "spark/render/Export.h"
#include "spark/render/Buffer.h"

namespace spark::render
{
    /**
     * \brief Describes the element type of an index buffer.
     */
    enum class IndexType
    {
        /// \brief Indices stored as 2-byte unsigned integers.
        UInt16 = 0x00000010,

        /// \brief Indices stored as 4-byte unsigned integers.
        UInt32 = 0x00000020
    };

    /**
     * \brief Describes the layout of an index buffer.
     */
    class SPARK_RENDER_EXPORT IIndexBufferLayout : public IBufferLayout
    {
    public:
        ~IIndexBufferLayout() noexcept override = default;

        /**
         * \brief Gets the index type of the buffer.
         * \return The type of the indices in the buffer.
         */
        [[nodiscard]] virtual IndexType indexType() const noexcept = 0;
    };

    /**
     * \brief Describes an index buffer.
     */
    class SPARK_RENDER_EXPORT IIndexBuffer : public virtual IBuffer
    {
    public:
        ~IIndexBuffer() noexcept override = default;

        /**
         * \brief Gets the layout of the buffer.
         * \return The layout of the buffer.
         */
        [[nodiscard]] virtual const IIndexBufferLayout& layout() const noexcept = 0;
    };
}
