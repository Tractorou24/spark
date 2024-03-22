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
        registerType<core::components::StaticCollider>();
        registerType<core::components::DynamicCollider>();
        registerType<core::components::Image>();
        registerType<core::components::Rectangle>();
        registerType<core::components::Text>();
        registerType<core::components::Transform>();
    }
}
