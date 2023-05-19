#pragma once

#include "spark/rtti/Export.h"
#include "spark/rtti/RttiBase.h"

#include <memory>
#include <string>
#include <unordered_map>

template <typename Type, typename... ParentTypes>
class Rtti;

namespace spark::rtti
{
    class SPARK_RTTI_EXPORT RttiDatabase
    {
        template <typename Type, typename... ParentTypes>
        friend class Rtti;

    public:
        /**
         * \brief Gets an RTTI instance by class name
         * \param class_name The class name we want the RTTI for
         * \return The RTTI instance
         */
        static RttiBase* Get(const std::string& class_name);

    private:
        /**
         * \brief Internally gets an RTTI instance by type
         * \tparam Type The type of the class we want to get the RTTI for
         * \tparam BaseTypes The parent types of this class
         * \return The Rtti data for this type
         */
        template <typename Type, typename... BaseTypes>
        static Rtti<Type, BaseTypes...>& get();

        /**
         * \return The storage map containing all the process RTTI data
         */
        static std::unordered_map<std::string, std::unique_ptr<RttiBase>>& dataStorage();
    };
}

#include "spark/rtti/impl/RttiDatabase.h"
