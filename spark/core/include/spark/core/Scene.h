#pragma once

#include "spark/core/Export.h"
#include "spark/core/GameObject.h"

#include "experimental/ser/SerializerScheme.h"
#include "spark/lib/Uuid.h"
#include "spark/rtti/HasRtti.h"

namespace spark::core
{
    class SPARK_CORE_EXPORT Scene final : public rtti::HasRtti
    {
        DECLARE_SPARK_RTTI(Scene)
        SPARK_ALLOW_PRIVATE_SERIALIZATION

    public:
        explicit Scene(GameObject* scene_root);
        ~Scene() override;

        Scene(const Scene& other) = delete;
        Scene(Scene&& other) noexcept = default;
        Scene& operator=(const Scene& other) = delete;
        Scene& operator=(Scene&& other) noexcept = default;

        /**
         * \brief Gets the UUID of the Scene.
         * \return A const reference to the UUID of this Scene.
         */
        [[nodiscard]] const lib::Uuid& uuid() const;

        /**
         * \brief Gets the root GameObject of the Scene.
         * \return A reference to the root GameObject.
         */
        [[nodiscard]] GameObject* root();

        /**
         * \brief Method called when the Scene is loaded.
         */
        void onLoad();

        /**
         * \brief Method called on every frame.
         */
        void onUpdate(float dt);

        /**
         * \brief Method called on every frame after \ref onUpdate to render the Scene.
         */
        void onRender();

        /**
         * \brief Method called when the Scene is unloaded.
         */
        void onUnload();

    private:
        lib::Uuid m_uuid;
        GameObject* m_root = nullptr;
        bool m_isLoaded = false;
    };
}

IMPLEMENT_SPARK_RTTI(spark::core::Scene)
