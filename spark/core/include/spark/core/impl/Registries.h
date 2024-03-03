#pragma once

namespace spark::core
{
    template <typename T> requires std::derived_from<T, core::GameObject>
    void GameObjectRegistry::registerType()
    {
        using Parent = patterns::Factory<std::string, core::GameObject, std::string, core::GameObject*>;
        Parent::registerType<T>(T::classRtti().className());
    }

    template <typename T> requires std::derived_from<T, core::Component>
    void ComponentRegistry::registerType()
    {
        using Parent = patterns::Factory<std::string, core::Component, core::GameObject*>;
        Parent::registerType<T>(T::classRtti().className());
    }
}
