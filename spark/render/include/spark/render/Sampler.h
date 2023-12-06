#pragma once

#include "spark/render/Export.h"
#include "spark/render/StateResource.h"

#include <array>

namespace spark::render
{
    /**
     * \brief Describes the filter operation when accessing a pixel from a texture coordinate.
     */
    enum class FilterMode
    {
        /// \brief Take the nearest texel with respect to the texture coordinate.
        Nearest = 0x00000001,

        /// \brief Linearly interpolate between the two closest texels with respect to the texture coordinate.
        Linear = 0x00000002
    };

    /**
     * \brief Describes the filter operation between two mip-map levels.
     */
    enum class MipMapMode
    {
        /// \brief Take the texel from the mip-map level that is closest to the actual depth.
        Nearest = 0x00000001,

        /// \brief Linearly interpolate between the texels of the two neighboring mip-map levels.
        Linear = 0x00000002
    };

    /**
     * \brief Describes how to treat texture coordinates that are outside the domain `[0..1]`.
     */
    enum class BorderMode
    {
        /// \brief Repeat the texture.
        Repeat = 0x00000001,

        /// \brief Mirror the texture.
        RepeatMirrored = 0x00010001,

        /// \brief Take the closest edge texel.
        ClampToEdge = 0x00000002,

        /// \brief Take the closest edge texel from the opposite site.
        ClampToEdgeMirrored = 0x00010002,

        /// \brief Return a pre-specified border color.
        ClampToBorder = 0x00000003,
    };

    /**
     * \brief Describes a texture sampler.
     */
    class SPARK_RENDER_EXPORT ISampler : public virtual IStateResource
    {
    public:
        ~ISampler() noexcept override = default;

        /**
         * \brief Gets the filter mode for minifying lookups.
         * \return A \ref FilterMode representing the filter mode for minifying lookups.
         */
        [[nodiscard]] virtual FilterMode minifyingFilter() const noexcept = 0;

        /**
         * \brief Gets the filter mode for magnifying lookups.
         * \return A \ref FilterMode representing the filter mode for magnifying lookups.
         */
        [[nodiscard]] virtual FilterMode magnifyingFilter() const noexcept = 0;

        /**
         * \brief Gets the addressing mode at the horizontal, vertical and depth borders.
         * \return A \ref std::array of \ref BorderMode representing the addressing modes according to the border.
         *
         * \note The returned array is ordered as follows: `[horizontal (U), vertical (V), depth (W)]`.
         */
        [[nodiscard]] virtual std::array<BorderMode, 3> borderMode() const noexcept = 0;

        /**
         * \brief Gets the anisotropy level used when sampling the texture.
         * \return The anisotropy level used when sampling the texture.
         *
         * \note A value of `0.0f` means that no anisotropic filtering is used.
         */
        [[nodiscard]] virtual float anisotropy() const noexcept = 0;

        /**
         * \brief Gets the mip-map selection mode.
         * \return A \ref MipMapMode representing the mip-map selection mode.
         */
        [[nodiscard]] virtual MipMapMode mipMapMode() const noexcept = 0;

        /**
         * \brief Gets the mip-map level of detail bias.
         * \return The mip-map level of detail bias.
         */
        [[nodiscard]] virtual float mipMapBias() const noexcept = 0;

        /**
         * \brief Gets the minimum texture level of detail.
         * \return The minimum texture level of detail.
         */
        [[nodiscard]] virtual float minLod() const noexcept = 0;

        /**
         * \brief Gets the maximum texture level of detail.
         * \return The maximum texture level of detail.
         */
        [[nodiscard]] virtual float maxLod() const noexcept = 0;
    };
}
