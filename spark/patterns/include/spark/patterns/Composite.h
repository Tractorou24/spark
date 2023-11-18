#pragma once

#include <memory>
#include <vector>

namespace spark::patterns
{
    template <typename DerivedType, template<typename> typename Deleter = std::default_delete>
    class Composite
    {
    public:
        explicit Composite(DerivedType* parent);
        virtual ~Composite();

        Composite(const Composite& other) = delete;
        Composite(Composite&& other) noexcept = default;
        Composite& operator=(const Composite& other) = delete;
        Composite& operator=(Composite&& other) noexcept = default;

        [[nodiscard]] std::vector<DerivedType*> getChildren() const;

        [[nodiscard]] DerivedType* getParent();
        [[nodiscard]] const DerivedType* getParent() const;

        [[nodiscard]] DerivedType* getRoot();
        [[nodiscard]] const DerivedType* getRoot() const;

    private:
        void add(DerivedType* child);
        void remove(DerivedType* child);
        void setParent(DerivedType* parent);

    private:
        DerivedType* m_parent = nullptr;
        std::vector<DerivedType*> m_children;
    };
}

#include "spark/patterns/impl/Composite.h"
