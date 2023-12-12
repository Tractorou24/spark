#pragma once

#include "spark/render/Export.h"
#include "spark/render/Pipeline.h"
#include "spark/render/ShaderStages.h"

#include <algorithm>
#include <memory>
#include <string>

namespace spark::render
{
    /**
     * \brief Interface representing a single shader module, a part of a \ref IShaderProgram.
     * \note A IShaderModule corresponds to a single source file.
     */
    class SPARK_RENDER_EXPORT IShaderModule
    {
    public:
        virtual ~IShaderModule() noexcept = default;

        /**
         * \brief Gets the type of the shader.
         * \return A \ref ShaderStage representing the stage of the pipeline this shader is used in.
         */
        [[nodiscard]] virtual ShaderStage stage() const noexcept = 0;

        /**
         * \brief Gets the name of the file this shader was loaded from.
         * \return A \ref std::string containing the name of the file this shader was loaded from.
         */
        [[nodiscard]] virtual const std::string& fileName() const noexcept = 0;

        /**
         * \brief Gets the entry point of this shader.
         * \return A \ref std::string containing the name of the entry point of this shader.
         */
        [[nodiscard]] virtual const std::string& entryPoint() const noexcept = 0;
    };

    /**
     * \brief Interface representing a shader program, a collection of \ref IShaderModule.
     */
    class SPARK_RENDER_EXPORT IShaderProgram
    {
    public:
        virtual ~IShaderProgram() noexcept = default;

        /**
         * \brief Gets all shaders contained in this program.
         * \return A \ref std::vector of pointers to all \link IShaderModule shaders \endlink contained in this program.
         */
        [[nodiscard]] std::vector<const IShaderModule*> shaders() const noexcept { return genericShaders(); }

        /**
         * \brief Uses shader reflexion to extract the pipeline layout from this shader program.
         * \return The \link IPipelineLayout pipeline layout \endlink extracted from this shader program.
         *
         * Shader reflection is not able to restore:
         * - Immutable sampler states in Vulkan.
         *
         * \note This may not be available on all backends.
         */
        [[nodiscard]] virtual std::shared_ptr<IPipelineLayout> reflectPipelineLayout() const = 0;

    private:
        /// \brief Private method to allow the replacement of the return type of \ref shaders() without complaining about non covariant return types.
        [[nodiscard]] virtual std::vector<const IShaderModule*> genericShaders() const noexcept = 0;
    };
}
