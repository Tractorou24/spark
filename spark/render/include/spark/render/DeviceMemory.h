#pragma once

#include "spark/render/Export.h"

#include <cstddef>

namespace spark::render
{
    /**
     * \brief Describes a chunk of memory on the GPU.
     */
    class SPARK_RENDER_EXPORT IDeviceMemory
    {
    public:
        virtual ~IDeviceMemory() noexcept = default;

        /**
         * \brief Get the number of sub-resources in the memory chunk.
         * \return The number of array elements in the memory chunk.
         *
         * \note For buffers, this equals the number of array elements. For images, this equals the product of layers, levels and planes.
         * This number represents the number of states, that can be obtained by calling the \ref IDeviceMemory::state() method.
         */
        [[nodiscard]] virtual unsigned int elements() const noexcept = 0;

        /**   
         * \brief Gets the size (in bytes) of the memory chunk.
         * \return The size of the memory chunk in bytes.
         *
         * \note The size of the device memory block depends on different factors. The actual used memory of one element can be obtained by calling by the \ref IDeviceMemory::elementSize().
         * For different reasons, though, elements may be required to be aligned to a certain size. The size of one aligned element is returned by \ref IDeviceMemory::alignedElementSize().
         * The size of the memory block, the elements get aligned to is returned by \ref IDeviceMemory::elementAlignment().
         */
        [[nodiscard]] virtual std::size_t size() const noexcept = 0;

        /**
         * \brief Gets the size (in bytes) of one element in the memory chunk. If there is only one element, this equals the result of \ref IDeviceMemory::size().
         * \return The size of one element in the memory chunk.
         *
         * \note For images, this method will return a value that equals the result of \ref IDeviceMemory::size().
         */
        [[nodiscard]] virtual std::size_t elementSize() const noexcept = 0;

        /**
         * \brief Gets the alignment of one element in the memory chunk.
         * \return The alignment of one element in the memory chunk.
         */
        [[nodiscard]] virtual std::size_t elementAlignment() const = 0;

        /**
         * \brief Gets the real size (in bytes) of the memory chunk.
         * \return The real size of the memory chunk in bytes.
         *
         * \note For images, this method will return a value that equals the result of \ref IDeviceMemory::size(), aligned by \ref IDeviceMemory::elementAlignment().
         */
        [[nodiscard]] virtual std::size_t alignedElementSize() const noexcept = 0;

        /**
         * \brief Checks if the resource can be bound to a read/write descriptor.
         * \return `true` if the resource can be bound to a read/write descriptor, `false` otherwise.
         *
         * \note If the resource is not writable, attempting to bind it to a writable descriptor will result in an exception.
         */
        [[nodiscard]] virtual bool writable() const noexcept = 0;
    };
}
