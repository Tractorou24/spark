#pragma once

#include "spark/core/Export.h"
#include "spark/core/Component.h"
#include "spark/core/GameObject.h"

#include "spark/patterns/Factory.h"
#include "spark/rtti/Rtti.h"

namespace spark::core
{
    /**
     * \brief A registry used to create \ref GameObject instances from their RTTI.
     *
     * This is a factory used to deserialize \link GameObject game objects \endlink instances from a game save.
     */
    class SPARK_CORE_EXPORT GameObjectRegistry final : public patterns::Factory<std::string, core::GameObject, std::string, core::GameObject*>
    {
    public:
        /**
         * \brief Instantiate a new \ref GameObjectRegistry with the default \ref GameObject types.
         */
        explicit GameObjectRegistry();

        /**
         * \brief Registers a new \ref GameObject into the registry.
         * \tparam T The type of the \ref GameObject to register.
         */
        template <typename T> requires std::derived_from<T, core::GameObject>
        void registerType();
    };

    /**
      * \brief A registry used to create \ref Component instances from their RTTI.
      *
      * This is a factory used to deserialize \link Component components \endlink instances from a game save.
      */
    class SPARK_CORE_EXPORT ComponentRegistry final : public patterns::Factory<std::string, core::Component, core::GameObject*>
    {
    public:
        /**
         * \brief Instantiate a new \ref ComponentRegistry with the default \ref Component types.
         */
        explicit ComponentRegistry();

        /**
         * \brief Registers a new \ref Component into the registry.
         * \tparam T The type of the \ref Component to register.
         */
        template <typename T> requires std::derived_from<T, core::Component>
        void registerType();
    };
}

#include "spark/core/impl/Registries.h"
