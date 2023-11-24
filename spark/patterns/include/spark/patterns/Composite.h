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

        [[nodiscard]] std::vector<DerivedType*> children() const;

        [[nodiscard]] DerivedType* parent();
        [[nodiscard]] const DerivedType* parent() const;

        [[nodiscard]] DerivedType* root();
        [[nodiscard]] const DerivedType* root() const;

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
