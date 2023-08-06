#include "spark/engine/Component.h"
#include "spark/base/Exception.h"

namespace spark::engine
{
    Component::Component(GameObject* parent)
        : m_gameObject(parent)
    {
        if (!parent)
            throw base::BadArgumentException("Cannot create a component with a null parent GameObject");
    }

    const lib::Uuid& Component::getUuid() const
    {
        return m_uuid;
    }

    GameObject* Component::getGameObject()
    {
        return m_gameObject;
    }

    const GameObject* Component::getGameObject() const
    {
        return m_gameObject;
    }
}
