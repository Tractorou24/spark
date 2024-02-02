#include "spark/patterns/Traverser.h"

#include "gtest/gtest.h"

namespace spark::patterns::testing
{
    class Node final : public Composite<Node>
    {
    public:
        bool touched = false;
        int touchedIndex = 0;

    public:
        explicit Node(Node* parent = nullptr) : Composite(parent) {}
    };

    TEST(TraverserShould, doNothingWithAnEmptyTree)
    {
        // Given an empty tree and a dummy traverser
        Node root;
        auto traverser = make_traverser<Node, bool&>([](Node*, bool& b) { b = true; });

        // When visiting the tree
        bool apply_called = false;
        traverse_tree<Node>(&root, traverser, apply_called);

        // Then nothing happens (no crash, no infinite loop, etc.)
        ASSERT_FALSE(apply_called);
    }

    TEST(TraverserShould, callApplyOnAllNodes)
    {
        /* Given a tree with multiple levels of nodes and a traverser
         *
         *           Node
         *         /  |  \
         *      Node Node Node
         *                /  \
         *             Node  Node
         *              /      \
         *           Node      Node
         */
        Node root;
        const Node a1(&root);
        const Node b2(&root);
        Node a3(&root);
        Node b1(&a3);
        const Node c1(&b1);
        const Node c2(&b1);
        const Node c3(&b1);

        auto traverser = make_traverser<Node, int&>([](Node*, int& apply) { apply++; });

        // When traversing the tree
        int apply = 0;
        traverse_tree<Node>(&root, traverser, apply);

        // Then all nodes are traversed and integer is incremented correctly
        EXPECT_EQ(apply, 7);
    }

    TEST(TraverserShould, callPreOnAllGroupsInOrder)
    {
        /* Given a tree with multiple levels of nodes and a traverser
         *
         *   Node
         *    |
         *   Node
         *    |
         *   Node
         *    |
         *   Node
         */
        Node root;
        Node a1(&root);
        Node b1(&a1);
        const Node c1(&b1);

        int count = 0;
        auto traverser = make_traverser<Node>([](Node*) {},
                                              [&count](Node* node)
                                              {
                                                  node->touched = true;
                                                  node->touchedIndex = count++;
                                              });

        // When traversing the tree
        traverse_tree<Node>(&root, traverser);

        // Then pre is called on all groups in order
        EXPECT_TRUE(a1.touched);
        EXPECT_EQ(a1.touchedIndex, 0);
        EXPECT_TRUE(b1.touched);
        EXPECT_EQ(b1.touchedIndex, 1);
    }

    TEST(TraverserShould, callPostOnAllGroupsInOrder)
    {
        /* Given a tree with multiple levels of nodes and a traverser
         *
         *   Node
         *    |
         *   Node
         *    |
         *   Node
         *    |
         *   Node
         */
        Node root;
        Node a1(&root);
        Node b1(&a1);
        const Node c1(&b1);

        int count = 0;
        auto traverser = make_traverser<Node>([](Node*) {},
                                              [](Node*) {},
                                              [&count](Node* node)
                                              {
                                                  node->touched = true;
                                                  node->touchedIndex = count++;
                                              });

        // When traversing the tree
        traverse_tree<Node>(&root, traverser);

        // Then post is called on all groups in order
        EXPECT_TRUE(b1.touched);
        EXPECT_EQ(b1.touchedIndex, 0);
        EXPECT_TRUE(a1.touched);
        EXPECT_EQ(a1.touchedIndex, 1);
    }

    TEST(TraverserShould, callPrePostApplyInOrder)
    {
        /* Given a tree with multiple levels of nodes and a traverser
        *
        *   Node
        *    |
        *   Node
        *    |
        *   Node
        *    |
        *   Node
        */
        Node root;
        Node a1(&root);
        Node b1(&a1);
        const Node c1(&b1);

        auto traverser = make_traverser<Node, std::stringstream&>([](Node*, std::stringstream& ss) { ss << "apply "; },
                                                                  [](Node*, std::stringstream& ss) { ss << "pre "; },
                                                                  [](Node*, std::stringstream& ss) { ss << "post "; });

        // When traversing the tree
        std::stringstream ss;
        traverse_tree<Node>(&root, traverser, ss);

        // Then pre, post and apply are called in order
        EXPECT_EQ(ss.str(), "pre apply pre apply apply post post ");
    }

    TEST(TraverserShould, traverseAConstTree)
    {
        /* Given a tree with multiple levels of nodes and a traverser
         *
         *   Node
         *    |
         *   Node
         *    |
         *   Node
         */
        Node root;
        Node a1(&root);
        const Node b1(&a1);

        auto traverser = make_traverser<const Node, std::stringstream&>([](const Node*, std::stringstream& ss) { ss << "apply "; },
                                                                        [](const Node*, std::stringstream& ss) { ss << "pre "; },
                                                                        [](const Node*, std::stringstream& ss) { ss << "post "; });

        // When traversing the tree as a const tree
        std::stringstream ss;
        [](const Node* c, auto& t, auto& s)
        {
            traverse_tree<const Node>(c, t, s);
        }(&root, traverser, ss);

        // Then pre, post and apply are called in order on the const tree
        EXPECT_EQ(ss.str(), "pre apply apply post ");
    }

    TEST(TraverserShould, perfectlyForwardArguments)
    {
        /* Given a tree with multiple levels of nodes and a traverser
         *
         *   Node
         *    |
         *   Node
         */
        Node root;
        const Node a1(&root);

        auto traverser = make_traverser<Node, int&>([](Node*, int& val)
        {
            val++;
        });

        // When traversing the tree
        int val = 0;
        traverse_tree<Node, int&>(&root, traverser, val);

        // Then, the given arguments can be modified and keep their fully qualified types
        EXPECT_EQ(val, 1);
    }
}
