#pragma once

namespace spark::core
{
    template <typename T> requires std::derived_from<T, core::GameObject>
    void GameObjectRegistry::registerType()
    {
        patterns::Factory<std::string, core::GameObject, std::string, core::GameObject*>::registerType<T>(T::classRtti().className());
        SerializationRegistry::registerType<T>(T::classRtti().className());
    }

    template <typename T> requires std::derived_from<T, core::Component>
    void ComponentRegistry::registerType()
    {
        patterns::Factory<std::string, core::Component, core::GameObject*>::registerType<T>(T::classRtti().className());
        SerializationRegistry::registerType<T>(T::classRtti().className());
    }
}
