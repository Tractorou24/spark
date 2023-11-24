#pragma once

#include "spark/rtti/Export.h"

#include "spark/base/Macros.h"

#include <string>
#include <vector>

namespace spark::rtti
{
    /**
     * \brief The abstract class used as base for the RTTI data
     */
    class SPARK_RTTI_EXPORT RttiBase
    {
    public:
        explicit RttiBase(std::string class_name);
        virtual ~RttiBase() = default;

        RttiBase(const RttiBase& other) = default;
        RttiBase(RttiBase&& other) noexcept = default;
        RttiBase& operator=(const RttiBase& other) = default;
        RttiBase& operator=(RttiBase&& other) noexcept = default;

        /**
         * \return The current class name as a string
         */
        [[nodiscard]] std::string className() const;

        /**
         * \return The RTTI information of all the parents for the current type
         */
        [[nodiscard]] virtual std::vector<RttiBase*> parents() = 0;

        /**
         * \brief Checks if the current RTTI inherits from a specific parent
         * \param potential_parent_type The parent type we want to check
         * \return True if the potentialParentType is in one of the parents, else False
         */
        [[nodiscard]] virtual bool isSubTypeOf(const RttiBase* potential_parent_type) = 0;

    private:
        // warning C4251: 'spark::rtti::RttiBase::m_className': class 'std::basic_string<_Elem,_Traits,_Ax>' needs to have dll-interface to be used by clients of class 'spark::rtti::RttiBase'
        SPARK_SUPPRESS_MSVC_WARNING(4251)
        std::string m_className;
    };
}
