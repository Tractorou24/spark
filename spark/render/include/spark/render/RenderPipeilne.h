#pragma once

#include "spark/render/Export.h"
#include "spark/render/InputAssembler.h"
#include "spark/render/Pipeline.h"
#include "spark/render/Rasterizer.h"

#include <memory>

namespace spark::render
{
    /**
     * \brief Interface for a render pipeline.
     */
    class SPARK_RENDER_EXPORT IRenderPipeline : public virtual IPipeline
    {
    public:
        ~IRenderPipeline() noexcept override = default;

        /**
         * \brief Gets the input assembler state used by the render pipeline.
         * \return A \ref std::shared_ptr to the \ref IInputAssembler state used by the render pipeline.
         */
        [[nodiscard]] std::shared_ptr<IInputAssembler> inputAssembler() const noexcept { return genericInputAssembler(); }

        /**
         * \brief Gets the rasterizer state used by the render pipeline.
         * \return A \ref std::shared_ptr to the \ref IRasterizer state used by the render pipeline.
         */
        [[nodiscard]] std::shared_ptr<IRasterizer> rasterizer() const noexcept { return genericRasterizer(); }

        /**
         * \brief Checks if the render pipeline is using alpha to coverage.
         * \return `true` if the render pipeline is using alpha to coverage, `false` otherwise.
         *
         * Alpha-to-Coverage is a multi-sampling technique used for partially transparent sprites or textures (such as foliage) to prevent visible flickering 
         * along edges. If enabled, the alpha-channel of the first (non-depth/stencil) render target is used to generate a temporary coverage mask that is combined
         * with the fragment coverage mask using a logical \c AND.
         */
        [[nodiscard]] virtual bool alphaToCoverage() const noexcept = 0;

    private:
        /// @{
        /// \brief Private method used to allow replacement of the generic methods by custom types.
        [[nodiscard]] virtual std::shared_ptr<IInputAssembler> genericInputAssembler() const noexcept = 0;
        [[nodiscard]] virtual std::shared_ptr<IRasterizer> genericRasterizer() const noexcept = 0;
        /// @}
    };

    /**
     * \brief Represents a graphics \ref IPipeline.
     * \tparam InputAssemblerType Type of the input assembler state used by the render pipeline. (inherits from \ref IInputAssembler)
     * \tparam RasterizerType Type of the rasterizer state used by the render pipeline. (inherits from \ref IRasterizer)
     */
    template <typename PipelineLayoutType, typename ShaderProgramType, typename InputAssemblerType, typename RasterizerType>
    class RenderPipeline : public IRenderPipeline, public StateResource, public virtual Pipeline<PipelineLayoutType, ShaderProgramType>
    {
    public:
        using input_assembler_type = InputAssemblerType;
        using rasterizer_type = RasterizerType;

    public:
        /// \copydoc IRenderPipeline::inputAssembler()
        [[nodiscard]] virtual std::shared_ptr<input_assembler_type> inputAssembler() const noexcept = 0;

        /// \copydoc IRenderPipeline::rasterizer()
        [[nodiscard]] virtual std::shared_ptr<rasterizer_type> rasterizer() const noexcept = 0;

    private:
        [[nodiscard]] std::shared_ptr<IInputAssembler> genericInputAssembler() const noexcept final { return inputAssembler(); }
        [[nodiscard]] std::shared_ptr<IRasterizer> genericRasterizer() const noexcept final { return rasterizer(); }
    };
}
