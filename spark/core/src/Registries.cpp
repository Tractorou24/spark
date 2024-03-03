#include "spark/core/Registries.h"

namespace spark::core
{
    GameObjectRegistry::GameObjectRegistry()
    {
        // Register all core game objects
        registerType<core::GameObject>();
    }
}
