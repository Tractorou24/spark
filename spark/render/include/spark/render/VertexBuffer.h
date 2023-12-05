#pragma once

#include "spark/render/Buffer.h"
#include "spark/render/Export.h"

#include <vector>

namespace spark::render
{
    /**
    * \brief Describes the layout of a vertex buffer.
    */
    class SPARK_RENDER_EXPORT IVertexBufferLayout : public IBufferLayout
    {
    public:
        ~IVertexBufferLayout() noexcept override = default;

        /**
         * \brief Gets the attributes of the vertex buffer.
         * \return A \ref std::vector of \ref BufferAttribute representing the attributes of the vertex buffer.
         */
        [[nodiscard]] virtual std::vector<const BufferAttribute*> attributes() const noexcept = 0;

        /**
         * \brief Adds an attribute to the vertex buffer layout.
         * \param attribute The \ref BufferAttribute to add.
         */
        virtual void addAttribute(BufferAttribute&& attribute) noexcept = 0;
    };

    /**
     * \brief Represents a vertex buffer.
     */
    class SPARK_RENDER_EXPORT IVertexBuffer : public virtual IBuffer
    {
    public:
        ~IVertexBuffer() noexcept override = default;

        /**
         * \brief Gets the layout of the vertex buffer.
         * \return A \ref IVertexBufferLayout representing the layout of the vertex buffer.
         */
        [[nodiscard]] virtual const IVertexBufferLayout& layout() const noexcept = 0;
    };
}
