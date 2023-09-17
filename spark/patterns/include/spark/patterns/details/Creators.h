#pragma once

#include <memory>

namespace spark::patterns::details
{
    /**
     * \brief A base class for all creators.
     * \tparam BaseType The type of the base class.
     * \tparam Args The types of the arguments passed to the constructor of the derived class.
     */
    template <typename BaseType, typename... Args>
    class BaseCreator
    {
    public:
        using BasePtr = std::unique_ptr<BaseType>;

    public:
        virtual ~BaseCreator() = default;
        [[nodiscard]] virtual BasePtr create(Args&&... args) const = 0;
    };

    /**
     * \brief The creator for a derived class.
     * \tparam DerivedType The type of the derived class.
     * \tparam BaseType The type of the base class.
     * \tparam Args The types of the arguments passed to the constructor of the derived class.
     */
    template <typename BaseType, typename DerivedType, typename... Args>
    class DerivedCreator final : public BaseCreator<BaseType, Args...>
    {
    public:
        [[nodiscard]] typename BaseCreator<BaseType, Args...>::BasePtr create(Args&&... args) const override
        {
            return std::make_unique<DerivedType>(std::forward<Args>(args)...);
        }
    };
}
