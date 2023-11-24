#pragma once

#include "spark/rtti/RttiBase.h"

#include "spark/mpl/typelist.h"

namespace spark::rtti
{
    /**
     * \brief The RTTI object containing all the needed data
     * \tparam Type The type we want to get the RTTI data
     * \tparam BaseTypes The parent types of type Type
     */
    template <typename Type, typename... BaseTypes>
    class Rtti final : public RttiBase
    {
    public:
        using ParentTypes = mpl::typelist<BaseTypes...>;

    public:
        explicit Rtti();

        /**
         * \return The rtti instance for the current Type and BaseTypes
         */
        static Rtti& instance();

        [[nodiscard]] std::vector<RttiBase*> parents() override;

        [[nodiscard]] bool isSubTypeOf(const RttiBase* potential_parent_type) override;
    };
}

#include "spark/rtti/impl/Rtti.h"
