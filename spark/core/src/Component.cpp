#include "spark/core/Component.h"

#include "spark/base/Exception.h"

namespace spark::core
{
    Component::Component(GameObject* parent)
        : m_gameObject(parent)
    {
        if (!parent)
            throw base::BadArgumentException("Cannot create a component with a null parent GameObject");
    }

    const lib::Uuid& Component::uuid() const
    {
        return m_uuid;
    }

    GameObject* Component::gameObject()
    {
        return m_gameObject;
    }

    const GameObject* Component::gameObject() const
    {
        return m_gameObject;
    }
}
