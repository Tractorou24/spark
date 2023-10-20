#pragma once

#include "spark/patterns/Composite.h"
#include "spark/patterns/details/Traverser.h"

#include <functional>

namespace spark::patterns
{
    /**
     * @brief A base class for any traverser used by the @ref traverse_tree function.
     * @tparam NodeType Type of the nodes of the tree.
     * @tparam FnArgsTypes The types of arguments that will be passed to the pre/post/apply functions.
     */
    template <typename NodeType, typename... FnArgsTypes>
    class Traverser
    {
    public:
        using FnType = std::function<void(NodeType*, FnArgsTypes&&...)>;

        explicit Traverser(FnType apply, FnType pre = {}, FnType post = {})
            : m_pre(std::move(pre)), m_apply(std::move(apply)), m_post(std::move(post)) {}

        void apply(NodeType* child, FnArgsTypes&&... args)
        {
            if (!m_apply)
                return;
            std::invoke(m_apply, child, std::forward<FnArgsTypes>(args)...);
        }

        void pre(NodeType* child, FnArgsTypes&&... args)
        {
            if (!m_pre)
                return;
            std::invoke(m_pre, child, std::forward<FnArgsTypes>(args)...);
        }

        void post(NodeType* child, FnArgsTypes&&... args)
        {
            if (!m_post)
                return;
            std::invoke(m_post, child, std::forward<FnArgsTypes>(args)...);
        }

    private:
        FnType m_pre, m_apply, m_post;
    };

    /**
     * @brief A helper function to create a traverser from functions.
     * The apply function is called on every element of the tree except the root, while the pre/post functions are called only on the elements that can have children (except the root).
     *
     * @tparam NodeType Type of the nodes of the tree.
     * @tparam FnArgsTypes The types of arguments that will be passed to the pre/post/apply functions.
     * @param apply The function to call on every element of the tree except the root.
     * @param pre The function to call on every element of the tree that can have children before traversing its children (except the root).
     * @param post The function to call on every element of the tree that can have children after traversing its children (except the root).
     * @return A traverser that can be used by the @ref traverse_tree function.
     */
    template <typename NodeType, typename... FnArgsTypes>
    Traverser<NodeType, FnArgsTypes...> make_traverser(typename Traverser<NodeType, FnArgsTypes...>::FnType apply,
                                                       typename Traverser<NodeType, FnArgsTypes...>::FnType pre = {},
                                                       typename Traverser<NodeType, FnArgsTypes...>::FnType post = {})
    {
        return Traverser<NodeType, FnArgsTypes...>(std::move(apply), std::move(pre), std::move(post));
    }

    /**
     * @brief A helper function to traverse a tree.
     * The apply function is called on every element of the tree except the root, while the pre/post functions are called only on the elements that can have children (except the root).
     *
     * @tparam NodeType Type of the nodes of the tree.
     * @tparam FnArgsTypes The types of arguments that will be passed to the pre/post/apply functions.
     * @param parent The root of the tree to traverse.
     * @param traverser The traverser to use, created with the @ref make_traverser function.
     * @param context The arguments to pass to the pre/post/apply functions.
     *
     * @note Currently, argument types must be compatible with universal references. So, it does not works with pointers.
     */
    template <typename NodeType, typename... FnArgsTypes>
    void traverse_tree(NodeType* parent, Traverser<NodeType, FnArgsTypes...>& traverser, FnArgsTypes&&... context)
    {
        details::TreeTraverserCaller<NodeType, FnArgsTypes...>::exec(parent, traverser, std::forward<FnArgsTypes>(context)...);
    }
}
