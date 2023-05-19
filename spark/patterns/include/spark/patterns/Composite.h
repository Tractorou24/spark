#pragma once

#include <functional>
#include <vector>

namespace spark::patterns
{
    template <typename DerivedType>
    class Composite
    {
    public:
        explicit Composite(DerivedType* parent);
        virtual ~Composite() = default;

        Composite(const Composite& other) = delete;
        Composite(Composite&& other) noexcept = default;
        Composite& operator=(const Composite& other) = delete;
        Composite& operator=(Composite&& other) noexcept = default;

        void setParent(DerivedType* new_parent);
        void addChild(DerivedType* child);
        [[nodiscard]] std::vector<DerivedType*> getChildren() const;

        [[nodiscard]] DerivedType* getParent();
        [[nodiscard]] const DerivedType* getParent() const;

        [[nodiscard]] DerivedType& getRoot();
        [[nodiscard]] const DerivedType& getRoot() const;

        void traverse(std::function<void(DerivedType*)> fn);
        void traverse(std::function<void(const DerivedType*)> fn) const;

        void traverseUp(std::function<void(DerivedType*)> fn);
        void traverseUp(std::function<void(const DerivedType*)> fn) const;

    protected:
        void add(DerivedType* child);
        void remove(DerivedType* child);

    private:
        DerivedType* m_parent = nullptr;
        std::vector<DerivedType*> m_children;
    };
}

#include "spark/patterns/impl/Composite.h"
