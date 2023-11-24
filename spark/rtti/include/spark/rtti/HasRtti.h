#pragma once

#include "spark/rtti/Export.h"
#include "spark/rtti/RttiBase.h"
#include "spark/rtti/details/Rtti.h"

// ReSharper disable once CppUnusedIncludeDirective : Used by the macros. We don't want the client to include it each time.
#include "spark/rtti/Rtti.h"

namespace spark::rtti
{
    /**
     * \brief The class anything using the RTTI should implement
     */
    class SPARK_RTTI_EXPORT HasRtti
    {
    public:
        HasRtti() = default;
        virtual ~HasRtti() = default;

        HasRtti(const HasRtti& other) = default;
        HasRtti(HasRtti&& other) noexcept = default;
        HasRtti& operator=(const HasRtti& other) = default;
        HasRtti& operator=(HasRtti&& other) noexcept = default;

        /**
         * \return The RTTI object of the current class
         */
        [[nodiscard]] virtual RttiBase& rttiInstance() const = 0;
    };
}

/**
 * \brief Declares RTTI method in a class that implements RTTI
 * \param ... In first position, the type name of the class
 * \param ... From second to n, the base types of the class
 */
#define DECLARE_SPARK_RTTI(...) DETAILS_DECLARE_SPARK_RTTI(__VA_ARGS__)

/**
 * \brief Registers a class into the RTTI system
 * \param DerivedClass The typename for which we want to register the RTTI
 */
#define IMPLEMENT_SPARK_RTTI(ClassName) DETAILS_SPARK_RTTI_REGISTER_NAME(ClassName)

/**
 * \brief Defines RTTI for a templated class with N template parameters.
 * \param TplType The template class full qualified name.
 * \param ... The list of template parameters as sequences.
 */
#define IMPLEMENT_SPARK_RTTI_TPL(ClassName, ...) DETAILS_SPARK_RTTI_REGISTER_TPL(ClassName, __VA_ARGS__)
