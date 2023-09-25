#pragma once

#include "spark/base/Exception.h"

namespace spark::patterns
{
    template <typename DerivedType>
    Composite<DerivedType>::Composite(DerivedType* parent)
    {
        setParent(parent);
    }

    template <typename DerivedType>
    Composite<DerivedType>::~Composite()
    {
        if (m_parent)
            m_parent->remove(static_cast<DerivedType*>(this));
    }

    template <typename DerivedType>
    DerivedType* Composite<DerivedType>::getParent()
    {
        return m_parent;
    }

    template <typename DerivedType>
    const DerivedType* Composite<DerivedType>::getParent() const
    {
        return m_parent;
    }

    template <typename DerivedType>
    DerivedType& Composite<DerivedType>::getRoot()
    {
        auto* ptr = static_cast<DerivedType*>(this);
        while (ptr->getParent() != nullptr)
            ptr = ptr->getParent();
        return *ptr;
    }

    template <typename DerivedType>
    const DerivedType& Composite<DerivedType>::getRoot() const
    {
        const auto* ptr = static_cast<const DerivedType*>(this);
        while (ptr->getParent() != nullptr)
            ptr = ptr->getParent();
        return *ptr;
    }

    template <typename DerivedType>
    std::vector<DerivedType*> Composite<DerivedType>::getChildren() const
    {
        return m_children;
    }

    template <typename DerivedType>
    void Composite<DerivedType>::traverse(std::function<void(DerivedType*)> fn)
    {
        for (std::size_t i = 0; i < m_children.size(); ++i)
        {
            auto* child = m_children[i];
            child->traverse(fn);
        }
        std::invoke(fn, static_cast<DerivedType*>(this));
    }

    template <typename DerivedType>
    void Composite<DerivedType>::traverse(std::function<void(const DerivedType*)> fn) const
    {
        for (std::size_t i = 0; i < m_children.size(); ++i)
        {
            const auto* child = m_children[i];
            child->traverse(fn);
        }
        std::invoke(fn, static_cast<const DerivedType*>(this));
    }

    template <typename DerivedType>
    void Composite<DerivedType>::traverseUp(std::function<void(DerivedType*)> fn)
    {
        // Copying the pointer to avoids an overload error with the const version
        if (auto* parent = m_parent)
            parent->traverseUp(fn);
        std::invoke(fn, static_cast<DerivedType*>(this));
    }

    template <typename DerivedType>
    void Composite<DerivedType>::traverseUp(std::function<void(const DerivedType*)> fn) const
    {
        // Copying the pointer to a const one avoids an overload error with the non-const version
        if (const auto* parent = m_parent)
            parent->traverseUp(fn);
        std::invoke(fn, static_cast<const DerivedType*>(this));
    }

    template <typename DerivedType>
    void Composite<DerivedType>::add(DerivedType* child)
    {
        auto it = std::ranges::find(m_children, child);
        if (it != m_children.end())
            throw spark::base::BadArgumentException("Child already exists in the children list!");
        m_children.push_back(child);
    }

    template <typename DerivedType>
    void Composite<DerivedType>::remove(DerivedType* child)
    {
        const auto it = std::ranges::find(m_children, child);
        if (it == m_children.end())
            throw spark::base::BadArgumentException("Child could not be found in the children list!");
        m_children.erase(it);
    }

    template <typename DerivedType>
    void Composite<DerivedType>::setParent(DerivedType* parent)
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
