#pragma once

#include "spark/render/Export.h"

#include <memory>

namespace spark::render
{
    /**
     * \brief Describes the operation used to compare depth or stencil values during depth/stencil tests.
     * \see DepthStencilState
     */
    enum class CompareOperation
    {
        /// \brief The test always fails.
        Never = 0x00000000,

        /// \brief The test succeeds, if the current value is less than the stencil ref or previous depth value.
        Less = 0x00000001,

        /// \brief The test succeeds, if the current value is greater than the stencil ref or previous depth value.
        Greater = 0x0000002,

        /// \brief The test succeeds, if the current value is equal to the stencil ref or previous depth value.
        Equal = 0x00000003,

        /// \brief The test succeeds, if the current value is less or equal to the stencil ref or previous depth value.
        LessEqual = 0x00000004,

        /// \brief The test succeeds, if the current value is greater or equal to the stencil ref or previous depth value.
        GreaterEqual = 0x00000005,

        /// \brief The test succeeds, if the current value is not equal to the stencil ref or previous depth value.
        NotEqual = 0x00000006,

        /// \brief The test always succeeds.
        Always = 0x00000007
    };

    /**
     * \brief An operation that is applied to the stencil buffer.
     * \see DepthStencilState
     */
    enum class StencilOperation
    {
        /// \brief Keep the current stencil value.
        Keep = 0x00000000,

        /// \brief Set the stencil value to `0`.
        Zero = 0x00000001,

        /// \brief Replace the current stencil value with the stencil ref.
        Replace = 0x00000002,

        /// \brief Increment the current stencil value.
        IncrementClamp = 0x00000003,

        /// \brief Decrement the current stencil value.
        DecrementClamp = 0x00000004,

        /// \brief Bitwise invert the current stencil value.
        Invert = 0x00000005,

        /// \brief Increment the current stencil value and wrap it, if it goes out of bounds.
        IncrementWrap = 0x00000006,

        /// \brief Decrement the current stencil value and wrap it, if it goes out of bounds.
        DecrementWrap = 0x00000007
    };

    /**
     * \brief Stores the depth/stencil state of a see \ref IRasterizer.
     */
    class SPARK_RENDER_EXPORT DepthStencilState final
    {
    public:
        /**
         * \brief Describes the depth test state.
         */
        struct DepthState
        {
        public:
            /// \brief Specifies, if depth testing should be enabled (default: `true`).
            bool enable = true;

            /// \brief Specifies, if depth should be written (default: `true`).
            bool write = true;

            /// \brief The compare operation used to pass the depth test (default: `CompareOperation::Always`).
            CompareOperation operation = CompareOperation::Always;
        };

        /**
         * \brief Describes the rasterizer depth bias.
         *
         * The depth bias can be used to alter the depth value function, i.e. how the values within the depth buffer are distributed.
         * By default, the depth buffer uses an exponential function scale to increase precision for closer objects. The values provided with \ref depthBiasClamp,
         * \ref depthBiasConstantFactor and \ref depthBiasSlopeFactor are used to change the domain clamping, offset and steepness of the depth  value distribution.
         */
        struct DepthBias
        {
        public:
            /// \brief Specifies, if depth bias should be used (default: `false`).
            bool enable = false;

            /// \brief Specifies the depth bias clamp (default: `0.0`).
            float clamp = 0.f;

            /// \brief Specifies the depth bias slope factor (default: `0.0`).
            float slopeFactor = 0.f;

            /// Specifies the depth bias constant factor (default: `0.0`).
            float constantFactor = 0.f;
        };

        /**
         * \brief Describes a stencil test for either front or back faces.
         */
        struct StencilTest
        {
        public:
            /// \brief The operation to apply to the stencil buffer, if the stencil test fails (default: `StencilOperation::Keep`).
            StencilOperation stencilFailOp = StencilOperation::Keep;

            /// \brief The operation to apply to the stencil buffer, if the stencil test passes (default: `StencilOperation::Keep`).
            StencilOperation stencilPassOp = StencilOperation::Replace;

            /// \brief The operation to apply to the stencil buffer, if the depth test fails (default: `StencilOperation::Keep`).
            StencilOperation depthFailOp = StencilOperation::Keep;

            /// \brief The operation use for stencil testing (default: `CompareOperation::Never`).
            CompareOperation operation = CompareOperation::Never;
        };

        /**
         * \brief Describes the rasterizer stencil state.
         */
        struct StencilState
        {
        public:
            /// \brief Specifies, if stencil state should be used (default: `false`).
            bool enable = false;

            /// \brief Specifies the bits to write to the stencil state (default: `0xFF`).
            unsigned char writeMask = 0xFF;

            /// \brief Specifies the bits to read from the stencil state (default: `0xFF`).
            unsigned char readMask = 0xFF;

            /// \brief Describes the stencil test for faces that point towards the camera.
            StencilTest frontFace = {};

            /// \brief Describes the stencil test for faces that point away from the camera.
            StencilTest backFace = {};
        };

    public:
        /**
         * \brief Initializes a new rasterizer depth/stencil empty state.
         */
        explicit DepthStencilState() noexcept;

        /**
         * \brief Initializes a new rasterizer depth/stencil state.
         * \param depth_state The depth state of the rasterizer.
         * \param depth_bias The depth bias of the rasterizer.
         * \param stencil_state The stencil state of the rasterizer.
         */
        explicit DepthStencilState(const DepthState& depth_state, const DepthBias& depth_bias, const StencilState& stencil_state) noexcept;
        ~DepthStencilState() noexcept;

        DepthStencilState(const DepthStencilState& other);
        DepthStencilState(DepthStencilState&& other) noexcept;
        DepthStencilState& operator=(const DepthStencilState& other);
        DepthStencilState& operator=(DepthStencilState&& other) noexcept;

        /**
         * \brief Gets the depth state of the rasterizer.
         * \return The \ref DepthState of the rasterizer.
         */
        [[nodiscard]] DepthState& depthState() const noexcept;

        /**
         * \brief Gets the depth bias of the rasterizer.
         * \return The \ref DepthBias of the rasterizer.
         */
        [[nodiscard]] DepthBias& depthBias() const noexcept;

        /**
         * \brief Gets the stencil state of the rasterizer.
         * \return The \ref StencilState of the rasterizer.
         */
        [[nodiscard]] StencilState& stencilState() const noexcept;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
