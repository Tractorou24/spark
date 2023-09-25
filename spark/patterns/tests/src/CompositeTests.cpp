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
