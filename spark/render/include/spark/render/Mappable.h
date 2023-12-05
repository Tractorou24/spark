#pragma once

#include "spark/render/Export.h"

#include <span>

namespace spark::render
{
    /**
     * \brief Interface allowing data to be mapped into the object.
     */
    class SPARK_RENDER_EXPORT IMappable
    {
    public:
        virtual ~IMappable() noexcept = default;

        /**
         * \brief Maps the memory at \p data into the internal memory of the object.
         * \param data Address of the beginning of the data to map (in bytes).
         * \param size Size of the data to map.
         * \param element The array element to map the data to.
         */
        virtual void map(const void* data, std::size_t size, unsigned int element) = 0;

        /**
         * \brief Maps the memory blocks within \p data into the internal memory of the object.
         * \param data A \ref std::span of memory blocks to map.
         * \param element_size The size of each element in the span (in bytes).
         * \param first_element The first element to map the data to.
         */
        virtual void map(std::span<const void*> data, std::size_t element_size, unsigned int first_element) = 0;

        /**
         * \brief Maps the memory at \p data into the internal memory of the object.
         * \param data Address of the beginning of the data to map.
         * \param size Size of the data to map (in bytes).
         * \param element The array element to map the data to.
         * \param write If `true`, the data will be written to the object. If `false`, the data will be read from the object.
         */
        virtual void map(void* data, size_t size, unsigned int element = 0, bool write = true) = 0;

        /**
         * \brief Maps the memory blocks within \p data into the internal memory of the object.
         * \param data A \ref std::span of memory blocks to map.
         * \param element_size The size of each element in the span (in bytes).
         * \param first_element The first element to map the data to.
         * \param write `true` if the data will be written to the object. `false` if the data will be read from the object.
         */
        virtual void map(std::span<void*> data, size_t element_size, unsigned int first_element = 0, bool write = true) = 0;
    };
}
