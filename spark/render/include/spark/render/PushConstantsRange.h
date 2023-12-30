#pragma once

#include "spark/render/Export.h"
#include "spark/render/ShaderStages.h"

namespace spark::render
{
    /**
     * \brief Interface for a push constants range in a \link IPushConstantsLayout push constants layout \endlink.
     */
    class SPARK_RENDER_EXPORT IPushConstantsRange
    {
    public:
        virtual ~IPushConstantsRange() noexcept = default;

        /**
         * \brief Gets the shader space the push constants can be accessed from.
         * \return The shader space the push constants can be accessed from.
         */
        [[nodiscard]] virtual unsigned int space() const noexcept = 0;

        /**
         * \brief Gets the binding point or register, the push constants are made available at.
         * \return The binding point or register, the push constants are made available at.
         */
        [[nodiscard]] virtual unsigned int binding() const noexcept = 0;

        /**
         * \brief Gets the offset from the push constants backing memory block, the range starts at.
         * \return The offset from the push constants backing memory block, the range starts at.
         */
        [[nodiscard]] virtual unsigned int offset() const noexcept = 0;

        /**
         * \brief Gets the size (in bytes) of the range.
         * \return The size (in bytes) of the range.
         */
        [[nodiscard]] virtual unsigned int size() const noexcept = 0;

        /**
         * \brief Gets the shader stage(s), the range is accessible from.
         * \return The shader stage(s), the range is accessible from.
         */
        [[nodiscard]] virtual ShaderStage stage() const noexcept = 0;
    };
}
