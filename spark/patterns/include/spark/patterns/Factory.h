#pragma once

#include "spark/patterns/details/Creators.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace spark::patterns
{
    /**
     * \brief A factory class that creates objects of type BaseType.
     * \tparam Key The type of the key used to reference objects.
     * \tparam BaseType The type of the base class.
     * \tparam Args The types of the arguments passed to the constructor of the derived class.
     */
    template <typename Key, typename BaseType, typename... Args>
    class Factory
    {
    public:
        using BasePtr = std::unique_ptr<BaseType>;
        using CreatorPtr = std::unique_ptr<details::BaseCreator<BaseType, Args...>>;

    public:
        Factory() = default;
        virtual ~Factory() = default;

        Factory(const Factory& other) = delete;
        Factory(Factory&& other) noexcept = default;
        Factory& operator=(const Factory& other) = delete;
        Factory& operator=(Factory&& other) noexcept = default;

        /**
         * \brief Registers a type the factory can create.
         * \tparam TypeToRegister The type of the derived class to register.
         * \param key The key used to reference the object.
         */
        template <typename TypeToRegister>
        void registerType(const Key& key);

        /**
         * \brief Creates an object of type BaseType. Throws an exception if the type is not registered.
         * \param key The key of type Key used to create the object.
         * \param args The arguments passed to the constructor of the derived class.
         * \return A std::unique_ptr to the created object.
         */
        [[nodiscard]] BasePtr create(const Key& key, Args&&... args) const;

        /**
         * \brief Creates an object of type BaseType.
         * \param key The key of type Key used to create the object.
         * \param args The arguments passed to the constructor of the derived class.
         * \return A std::unique_ptr to the created object if the type is registered, else a nullptr.
         */
        [[nodiscard]] BasePtr createOrFail(const Key& key, Args&&... args) const noexcept;

        /**
         * \brief Gets a vector of all registered types in the factory.
         * \return The std::vector containing the keys of all registered types.
         */
        [[nodiscard]] std::vector<Key> getRegisteredTypes() const noexcept;

    private:
        std::unordered_map<Key, CreatorPtr> m_creators;
    };
}

#include "spark/patterns/impl/Factory.h"
