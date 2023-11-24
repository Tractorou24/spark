#pragma once

#include "spark/engine/Export.h"

#include "spark/base/Macros.h"
#include "spark/lib/Uuid.h"
#include "spark/rtti/HasRtti.h"

namespace spark::engine
{
    class GameObject;

    /**
     * \brief A component that can be attached to a GameObject to provide additional functionality.
     *
     * This class is the base for all components. It should not be used directly but rather inherited from to create a new component.
     */
    class SPARK_ENGINE_EXPORT Component : public rtti::HasRtti
    {
        DECLARE_SPARK_RTTI(Component)

    public:
        ~Component() override = default;

        Component(const Component& other) = delete;
        Component(Component&& other) noexcept = default;
        Component& operator=(const Component& other) = delete;
        Component& operator=(Component&& other) noexcept = default;

        /**
         * \brief Gets the UUID of the component.
         * \return A const reference to the UUID of the component.
         */
        [[nodiscard]] const lib::Uuid& uuid() const;

        /**
         * \brief Gets the GameObject that the component is attached to.
         * \return A pointer to the GameObject that the component is attached to.
         */
        [[nodiscard]] GameObject* gameObject();

        /**
         * \brief Gets the GameObject that the component is attached to.
         * \return A const pointer to the GameObject that the component is attached to.
         */
        [[nodiscard]] const GameObject* gameObject() const;

        /**
         * \brief Renders the component.
         */
        virtual void render() const {}

        /**
         * \brief Method called when the component is attached to a GameObject.
         */
        virtual void onAttach() {}

        /**
         * \brief Method called on every frame.
         * \param dt The time in seconds since the last frame.
         */
        virtual void onUpdate(float dt) { SPARK_UNUSED(dt); }

        /**
         * \brief Method called when the component is detached from a GameObject.
         */
        virtual void onDetach() {}

    protected:
        /**
         * \brief Instantiates a new Component.
         * \param parent The GameObject that the component will be attached to.
         */
        explicit Component(GameObject* parent);

    private:
        lib::Uuid m_uuid;
        GameObject* m_gameObject = nullptr;
    };
}

IMPLEMENT_SPARK_RTTI(spark::engine::Component)
