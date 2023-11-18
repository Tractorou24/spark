#include "gtest/gtest.h"

#include "spark/patterns/Composite.h"

namespace spark::patterns::testing
{
    class Node final : public Composite<Node>
    {
    public:
        explicit Node(Node* parent = nullptr)
            : Composite(parent) {}
    };

    template <typename Impl>
    struct CustomDeleter;

    class NodeWithDeleter final : public Composite<NodeWithDeleter, CustomDeleter>
    {
    public:
        explicit NodeWithDeleter(int& counter, const std::function<void()>& fn = {}, NodeWithDeleter* parent = nullptr)
            : Composite(parent), deleterCount(counter), function(fn) {}

        ~NodeWithDeleter() override { ++deleterCount; }

        int& deleterCount;
        std::function<void()> function;
    };

    template <typename Impl>
    struct CustomDeleter
    {
        void operator()(Impl* impl)
        {
            if (impl->function)
                impl->function();
            delete impl;
        }
    };

    TEST(CompositeShould, createASimpleTree)
    {
        // Given a root node
        Node root;

        // When adding children at the 1st and 2nd levels
        Node a1(&root);
        Node b1(&a1);

        // Then, the nodes are added in the tree
        const auto& root_children = root.getChildren();
        const auto& a1_children = a1.getChildren();
        EXPECT_TRUE(std::ranges::find(root_children, &a1) != root_children.end());
        EXPECT_TRUE(std::ranges::find(a1_children, &b1) != a1_children.end());
    }

    TEST(CompositeShould, deleteChildrenWhenParentIsDeleted)
    {
        // Given a simple tree with a deleter count
        int dtor_delete_count = 0;

        auto* root = new NodeWithDeleter(dtor_delete_count);
        new NodeWithDeleter(dtor_delete_count, {}, root);

        // When deleting the root node
        delete root;

        // Then, the children are deleted (destructors are called)
        EXPECT_EQ(dtor_delete_count, 2);
    }

    TEST(CompositeShould, useGivenDeleter)
    {
        // Given a simple tree with a deleter count and a function
        int dtor_delete_count = 0, delete_count = 0;
        auto fn = [&dtor_delete_count]() { ++dtor_delete_count; };

        auto* root = new NodeWithDeleter(delete_count, fn, nullptr);
        new NodeWithDeleter(delete_count, fn, root);

        // When deleting the root node with the custom deleter
        CustomDeleter<NodeWithDeleter>()(root);

        // Then, the children are deleted (destructors are called) and the function is called
        EXPECT_EQ(dtor_delete_count, 2);
        EXPECT_EQ(delete_count, 2);
    }

    TEST(CompositeShould, returnItsParent)
    {
        // Given a simple tree
        Node root, a1(&root);

        // When getting the parent of a node
        const auto* root_parent = root.getParent();
        const auto* a1_parent = a1.getParent();

        // Then, they are correct
        EXPECT_EQ(root_parent, nullptr);
        EXPECT_EQ(a1_parent, &root);
    }

    TEST(CompositeShould, returnItsRootNode)
    {
        // Given a multi level tree
        Node root, a1(&root), b1(&root);

        // When getting the root of the tree
        const auto& root_root = root.getRoot();
        const auto& a1_root = a1.getRoot();
        const auto& b1_root = b1.getRoot();

        // Then, they are correct
        EXPECT_EQ(root_root, &root);
        EXPECT_EQ(a1_root, &root);
        EXPECT_EQ(b1_root, &root);
    }
}
