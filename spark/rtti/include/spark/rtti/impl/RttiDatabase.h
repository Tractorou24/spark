#pragma once

#include "spark/rtti/details/Rtti.h"

namespace spark::rtti
{
    template <typename Type, typename... BaseTypes>
    Rtti<Type, BaseTypes...>& RttiDatabase::get()
    {
        constexpr auto class_name_array = details::GetClassName<Type>::exec();
        const std::string class_name(class_name_array.data(), class_name_array.size());

        auto& storage = dataStorage();
        if (!storage.contains(class_name))
            storage.insert_or_assign(class_name, std::make_unique<Rtti<Type, BaseTypes...>>());

        return *static_cast<Rtti<Type, BaseTypes...>*>(storage.at(class_name).get());
    }
}
