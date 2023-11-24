#pragma once

#include "spark/engine/Export.h"
#include "spark/engine/GameObject.h"

#include "spark/lib/Uuid.h"

#include "spark/rtti/HasRtti.h"

namespace spark::engine
{
    class SPARK_ENGINE_EXPORT Scene final : public rtti::HasRtti
    {
        DECLARE_SPARK_RTTI(Scene)

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
         * \brief Method called when the Scene is unloaded.
         */
        void onUnload();

    private:
        lib::Uuid m_uuid;
        GameObject* m_root;
        bool m_isLoaded = false;
    };
}

IMPLEMENT_SPARK_RTTI(spark::engine::Scene)

namespace spark::core
{
    template <typename T>
    struct Render2D;

    template <>
    struct Render2D<engine::Scene>
    {
        static void exec(engine::Scene& scene)
        {
            Render2D<engine::GameObject>::exec(*scene.root());
        }
    };
}
