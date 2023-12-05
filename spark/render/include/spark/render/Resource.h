#pragma once

namespace spark::render
{
    /**
     * \brief Provides access to a resource managed by the derived class.
     * \tparam Handle Type of the managed resource.
     */
    template <typename Handle>
    class IResource
    {
    public:
        virtual ~IResource() noexcept = default;

        /**
         * \brief Gets the  managed resource handle.
         * \return A const reference to the managed resource handle.
         */
        [[nodiscard]] virtual const Handle& handle() const = 0;

    protected:
        /**
         * \brief Gets the  managed resource handle.
         * \return A modifiable reference to the managed resource handle.
         */
        [[nodiscard]] virtual Handle& handle() = 0;
    };

    template <typename Handle>
    class Resource : public virtual IResource<Handle>
    {
    public:
        /**
         * \brief Initializes the managed resource with the given handle.
         * \param handle The handle to the managed resource.
         */
        explicit Resource(Handle handle) noexcept;
        ~Resource() noexcept override = default;

        Resource(const Resource& other) = delete;
        Resource(Resource&& other) noexcept = delete;
        Resource& operator=(const Resource& other) = delete;
        Resource& operator=(Resource&& other) noexcept = delete;

        /// \copydoc IResource::handle()
        [[nodiscard]] const Handle& handle() const final;

    protected:
        /// \copydoc IResource::handle()
        [[nodiscard]] Handle& handle() final;

    private:
        Handle m_handle;
    };
}

#include "spark/render/impl/Resource.h"
