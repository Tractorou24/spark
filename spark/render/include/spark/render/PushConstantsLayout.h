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
}
