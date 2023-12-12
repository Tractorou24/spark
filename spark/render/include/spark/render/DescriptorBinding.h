#pragma once

#include "spark/render/Buffer.h"
#include "spark/render/Export.h"
#include "spark/render/Image.h"
#include "spark/render/Sampler.h"

#include <functional>
#include <optional>
#include <variant>

namespace spark::render
{
    /**
     * \brief Describes a resource binding in a descriptor set.
     */
    struct SPARK_RENDER_EXPORT DescriptorBinding
    {
        /// \brief Binding point in the descriptor set.
        std::optional<unsigned int> binding = std::nullopt;

        /// \brief Resource to bind or \ref std::monostate if the binding is empty.
        std::variant<std::monostate, std::reference_wrapper<IBuffer>, std::reference_wrapper<IImage>, std::reference_wrapper<ISampler>> resource;

        /// \brief Index of the descriptor array at which the resource array starts.
        /// \note If the resource contains an array, the elements (layers for images) will be bound starting at this descriptor. The first bound element is \ref DescriptorBinding::firstElement. The number of elements bounded is \ref DescriptorBinding::elements.
        unsigned int firstDescriptor = 0;

        /// \brief Index of the first element (layer for images) of the resource array to bind, starting at \ref DescriptorBinding::firstDescriptor.
        /// \note If \ref DescriptorBinding::resource is a \ref ISampler, this property is ignored.
        unsigned int firstElement = 0;

        /// \brief Number of elements or image layers to bind, starting at \ref DescriptorBinding::firstDescriptor.
        /// \note If \ref DescriptorBinding::resource is a \ref ISampler, this property is ignored.
        unsigned int elements = 0;

        /// \brief If the resource is an \ref IImage, this property specifies the first level to bind.
        /// \note If \ref DescriptorBinding::resource is not an \ref IImage, this property is ignored.
        unsigned int firstLevel = 0;

        /// \brief If the resource is an \ref IImage, this property specifies the number of levels to bind.
        /// \note If \ref DescriptorBinding::resource is not an \ref IImage, this property is ignored.
        unsigned int levels = 0;
    };
}
