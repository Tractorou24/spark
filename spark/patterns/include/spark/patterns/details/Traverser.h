#pragma once

namespace spark::patterns::details
{
    /**
     * @brief A generic tree traverser caller that can be used to traverse any @ref Composite.
     *
     * This traverser operates on a @ref Composite data structure by recursively traversing its elements and performing designated operations.
     * To use this, you need to provide a @ref Traverser implementation that defines the pre/post/apply functions.
     *
     * The apply function is called on every element of the tree except the root, while the pre/post functions are called only on the elements that can have children.
     *
     * @tparam NodeType Type of the nodes of the tree.
     * @tparam FnArgsTypes The types of arguments that will be passed to the pre/post/apply functions.
     */
    template <typename NodeType, typename... FnArgsTypes>
    class TreeTraverserCaller
    {
    public:
        /**
         * @brief Traverses the given tree using the given traverser
         * @param container The tree to visit.
         * @param traverser The traverser implementation with the pre/post/apply functions.
         * @param args The arguments to pass to the pre/post/apply functions.
         */
        template <typename Traverser>
        static void exec(NodeType* container, Traverser& traverser, FnArgsTypes&&... args)
        {
            for (auto* child : container->children())
            {
                if (!child->children().empty())
                {
                    auto* sub_container = static_cast<std::conditional_t<std::is_const_v<NodeType>, const NodeType, NodeType>*>(child);
                    traverser.pre(child, std::forward<FnArgsTypes>(args)...);
                    traverser.apply(child, std::forward<FnArgsTypes>(args)...);
                    exec(sub_container, traverser, std::forward<FnArgsTypes>(args)...);
                    traverser.post(child, std::forward<FnArgsTypes>(args)...);
                } else
                    traverser.apply(child, std::forward<FnArgsTypes>(args)...);
            }
        }
    };
}
