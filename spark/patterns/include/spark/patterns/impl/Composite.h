#pragma once

#include "spark/base/Exception.h"

namespace spark::patterns
{
    template <typename DerivedType, template <typename> class Deleter>
    Composite<DerivedType, Deleter>::Composite(DerivedType* parent)
    {
        setParent(parent);
    }

    template <typename DerivedType, template<typename> typename Deleter>
    Composite<DerivedType, Deleter>::~Composite()
    {
        if (m_parent)
            m_parent->remove(static_cast<DerivedType*>(this));
        m_parent = nullptr;

        for (std::size_t i = 0; i < m_children.size(); ++i)
            Deleter<DerivedType>()(m_children[i]);
    }

    template <typename DerivedType, template<typename> typename Deleter>
    DerivedType* Composite<DerivedType, Deleter>::parent()
    {
        return m_parent;
    }

    template <typename DerivedType, template<typename> typename Deleter>
    const DerivedType* Composite<DerivedType, Deleter>::parent() const
    {
        return m_parent;
    }

    template <typename DerivedType, template<typename> typename Deleter>
    DerivedType* Composite<DerivedType, Deleter>::root()
    {
        auto* ptr = static_cast<DerivedType*>(this);
        while (ptr->parent() != nullptr)
            ptr = ptr->parent();
        return ptr;
    }

    template <typename DerivedType, template<typename> typename Deleter>
    const DerivedType* Composite<DerivedType, Deleter>::root() const
    {
        const auto* ptr = static_cast<const DerivedType*>(this);
        while (ptr->parent() != nullptr)
            ptr = ptr->parent();
        return ptr;
    }

    template <typename DerivedType, template<typename> typename Deleter>
    std::vector<DerivedType*> Composite<DerivedType, Deleter>::children() const
    {
        return m_children;
    }

    template <typename DerivedType, template<typename> typename Deleter>
    void Composite<DerivedType, Deleter>::add(DerivedType* child)
    {
        auto it = std::ranges::find(m_children, child);
        if (it != m_children.end())
            throw spark::base::BadArgumentException("Child already exists in the children list!");
        m_children.push_back(child);
    }

    template <typename DerivedType, template<typename> typename Deleter>
    void Composite<DerivedType, Deleter>::remove(DerivedType* child)
    {
        const auto it = std::ranges::find(m_children, child);
        if (it == m_children.end())
            throw spark::base::BadArgumentException("Child could not be found in the children list!");
        m_children.erase(it);
    }

    template <typename DerivedType, template<typename> typename Deleter>
    void Composite<DerivedType, Deleter>::setParent(DerivedType* parent)
    {
        // Remove from old parent if it not nullptr
        if (m_parent)
            m_parent->remove(static_cast<DerivedType*>(this));

        // Set new parent
        m_parent = parent;

        // Add this child to the new parent il it is not nullptr
        if (m_parent)
            m_parent->add(static_cast<DerivedType*>(this));
    }
}
