#include "spark/core/Registries.h"
#include "spark/core/details/SerializationSchemes.h"

namespace spark::core
{
    GameObjectRegistry::GameObjectRegistry()
    {
        // Register all core game objects
        registerType<core::GameObject>();
    }

    ComponentRegistry::ComponentRegistry()
    {
        // Register all core components
        registerType<core::Component>();
        registerType<core::components::Circle>();
        registerType<core::components::Collider>();
    }
}
