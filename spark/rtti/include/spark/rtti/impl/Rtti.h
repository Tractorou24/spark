#pragma once

#include "spark/rtti/RttiDatabase.h"
#include "spark/rtti/details/Rtti.h"

namespace spark::rtti
{
    template <typename Type, typename... BaseTypes>
    Rtti<Type, BaseTypes...>::Rtti(std::string class_name)
        : RttiBase(std::move(class_name)) {}

    template <typename Type, typename... BaseTypes>
    Rtti<Type, BaseTypes...>& Rtti<Type, BaseTypes...>::instance()
    {
        static Rtti& instance = RttiDatabase::get<Type, BaseTypes...>();
        return instance;
    }

    template <typename Type, typename... BaseTypes>
    std::vector<RttiBase*> Rtti<Type, BaseTypes...>::parents()
    {
        return details::GetParentRtti<BaseTypes...>::exec();
    }

    template <typename Type, typename... BaseTypes>
    bool Rtti<Type, BaseTypes...>::isSubTypeOf(const RttiBase* potential_parent_type)
    {
        if (potential_parent_type == this)
            return true;

        for (std::size_t i = 0; i < sizeof...(BaseTypes); ++i)
            if (parents()[i]->isSubTypeOf(potential_parent_type))
                return true;
        return false;
    }
}
