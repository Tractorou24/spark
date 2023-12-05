#pragma once

#include "spark/render/Export.h"

#include <memory>
#include <string>

namespace spark::render
{
    /**
     * \brief Interface for a state resource.
     */
    class SPARK_RENDER_EXPORT IStateResource
    {
    public:
        virtual ~IStateResource() noexcept = default;

        /**
         * \brief Gets the name of the state resource.
         * \return A \link std::string const std::string& \endlink containing the name of the state resource.
         */
        [[nodiscard]] virtual const std::string& name() const noexcept = 0;
    };

    /**
     * \brief Base class for a resource that can be identified by a name in a \ref DeviceState.
     */
    class SPARK_RENDER_EXPORT StateResource : public virtual IStateResource
    {
    public:
        /**
         * \brief Initializes a new \ref StateResource instance.
         * \param name The name of the resource.
         */
        explicit StateResource(std::string_view name) noexcept;
        ~StateResource() noexcept override;

        StateResource(const StateResource& other) = delete;
        StateResource(StateResource&& other) noexcept;
        StateResource& operator=(const StateResource& other) = delete;
        StateResource& operator=(StateResource&& other) noexcept;

        /// \copydoc IStateResource::name()
        [[nodiscard]] const std::string& name() const noexcept final;

    protected:
        /// \brief Initializes a new \ref StateResource instance with an empty name.
        explicit StateResource() noexcept;

        /// \copydoc IStateResource::name()
        [[nodiscard]] std::string& name() noexcept;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
