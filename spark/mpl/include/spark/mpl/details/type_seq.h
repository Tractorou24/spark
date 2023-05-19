#pragma once

namespace spark::mpl::details
{
    namespace impl
    {
        // "contains" implementation
        template <typename Type, typename... Types>
        struct TypeSeqContainsImpl;

        template <typename Type, typename FirstType, typename... OtherTypes>
        struct TypeSeqContainsImpl<Type, FirstType, OtherTypes...>
        {
            static constexpr bool value = std::is_same_v<Type, FirstType> || TypeSeqContainsImpl<Type, OtherTypes...>::value;
        };

        template <typename Type>
        struct TypeSeqContainsImpl<Type>
        {
            static constexpr bool value = false;
        };

        // "at" implementation
        template <int Pos, typename... Types>
        struct TypeSeqAtImpl;

        template <int Pos, typename FirstType, typename... OtherTypes>
        struct TypeSeqAtImpl<Pos, FirstType, OtherTypes...>
        {
            using type = typename TypeSeqAtImpl<Pos - 1, OtherTypes...>::type;
        };

        template <typename FirstType, typename... OtherTypes>
        struct TypeSeqAtImpl<0, FirstType, OtherTypes...>
        {
            using type = FirstType;
        };

        // remove implementation
        template <typename AcceptedTypelist, typename PendingTypeList, int Pos>
        struct TypeSeqRemoveImpl;

        template <template <typename...> class ContainerTpl, typename... AcceptedTypes, typename FirstPendingType, typename... OtherPendingTypes, int Pos>
        struct TypeSeqRemoveImpl<ContainerTpl<AcceptedTypes...>, ContainerTpl<FirstPendingType, OtherPendingTypes...>, Pos>
        {
            using type = typename TypeSeqRemoveImpl<ContainerTpl<AcceptedTypes..., FirstPendingType>, ContainerTpl<OtherPendingTypes...>, Pos - 1>::type;
        };

        template <template <typename...> class ContainerTpl, typename... AcceptedTypes, typename FirstPendingType, typename... OtherPendingTypes>
        struct TypeSeqRemoveImpl<ContainerTpl<AcceptedTypes...>, ContainerTpl<FirstPendingType, OtherPendingTypes...>, 0>
        {
            using type = ContainerTpl<AcceptedTypes..., OtherPendingTypes...>;
        };

        // convert implementation
        template <typename SequenceType, template <typename...> class DestContainerTpl>
        struct TypeSeqConvert;

        template <template <typename...> class SrcContainerTpl, typename... Types, template <typename...> class DestContainerTpl>
        struct TypeSeqConvert<SrcContainerTpl<Types...>, DestContainerTpl>
        {
            using type = DestContainerTpl<Types...>;
        };
    }


    // contains
    template <typename SequenceType, typename Type>
    struct TypeSeqContains;

    template <template <typename...> class ContainerTpl, typename... Types, typename Type>
    struct TypeSeqContains<ContainerTpl<Types...>, Type>
    {
        static constexpr bool value = impl::TypeSeqContainsImpl<Type, Types...>::value;
    };

    // at
    template <typename SequenceType, int Pos>
    struct TypeSeqAt;

    template <template <typename...> class ContainerTpl, typename... Types, int Pos>
    struct TypeSeqAt<ContainerTpl<Types...>, Pos>
    {
        using type = typename impl::TypeSeqAtImpl<Pos, Types...>::type;
    };

    // push_back
    template <typename SequenceType, typename NewType>
    struct TypeSeqPushBack;

    template <template <typename...> class ContainerTpl, typename... Types, typename NewType>
    struct TypeSeqPushBack<ContainerTpl<Types...>, NewType>
    {
        using type = ContainerTpl<Types..., NewType>;
    };

    // push_front
    template <typename SequenceType, typename Type>
    struct TypeSeqPushFront;

    template <template <typename...> class ContainerTpl, typename... Types, typename Type>
    struct TypeSeqPushFront<ContainerTpl<Types...>, Type>
    {
        using type = ContainerTpl<Type, Types...>;
    };

    // remove
    template <typename SequenceType, int Pos>
    struct TypeSeqRemove;

    template <template <typename...> class ContainerTpl, typename... Types, int Pos>
    struct TypeSeqRemove<ContainerTpl<Types...>, Pos>
    {
        using type = typename impl::TypeSeqRemoveImpl<ContainerTpl<>, ContainerTpl<Types...>, Pos>::type;
    };

    // pop_front
    template <typename SequenceType>
    struct TypeSeqPopFront;

    template <template <typename...> class ContainerTpl, typename FirstType, typename... Types>
    struct TypeSeqPopFront<ContainerTpl<FirstType, Types...>>
    {
        using type = ContainerTpl<Types...>;
    };

    // pop_back
    template <typename SequenceType>
    struct TypeSeqPopBack;

    template <template <typename...> class ContainerTpl, typename... Types>
    struct TypeSeqPopBack<ContainerTpl<Types...>>
    {
        using type = typename TypeSeqRemove<ContainerTpl<Types...>, sizeof...(Types) - 1>::type;
    };

    // transform
    template <typename TypeSequence, template <typename> class TransformationTpl>
    struct TypeSeqTransform;

    template <template <typename...> class ContainerTpl, typename... Types, template <typename> class TransformationTpl>
    struct TypeSeqTransform<ContainerTpl<Types...>, TransformationTpl>
    {
        using type = ContainerTpl<typename TransformationTpl<Types>::type...>;
    };

    // concat
    template <typename... SequenceTypes>
    struct TypeSeqConcat;

    template <template <typename...> class ContainerTpl, typename... Types>
    struct TypeSeqConcat<ContainerTpl<Types...>>
    {
        using type = ContainerTpl<Types...>;
    };

    template <template <typename...> class ContainerTpl, typename... FirstSequenceTypes, typename... SecondSequenceTypes>
    struct TypeSeqConcat<ContainerTpl<FirstSequenceTypes...>, ContainerTpl<SecondSequenceTypes...>>
    {
        using type = ContainerTpl<FirstSequenceTypes..., SecondSequenceTypes...>;
    };

    template <typename FirstSequence, typename SecondSequence, typename ThirdSequence, typename... OtherSequences>
    struct TypeSeqConcat<FirstSequence, SecondSequence, ThirdSequence, OtherSequences...>
    {
        using type = typename TypeSeqConcat<typename TypeSeqConcat<FirstSequence, SecondSequence>::type, typename TypeSeqConcat<ThirdSequence, OtherSequences...>::type>::type;
    };

    // front
    template <typename... SequenceTypes>
    struct TypeSeqFront;

    template <template <typename...> class ContainerTpl, typename FirstType, typename... Types>
    struct TypeSeqFront<ContainerTpl<FirstType, Types...>>
    {
        using type = FirstType;
    };

    // back
    template <typename... SequenceTypes>
    struct TypeSeqBack;

    template <template <typename...> class ContainerTpl, typename FirstType, typename... Types>
    struct TypeSeqBack<ContainerTpl<FirstType, Types...>>
    {
        using type = typename TypeSeqBack<ContainerTpl<Types...>>::type;
    };

    template <template <typename...> class ContainerTpl, typename FirstType>
    struct TypeSeqBack<ContainerTpl<FirstType>>
    {
        using type = FirstType;
    };

    namespace type_seq
    {
        template <typename SequenceType, typename Type>
        using contains = TypeSeqContains<SequenceType, Type>;

        template <typename SequenceType, int Pos>
        using at = typename TypeSeqAt<SequenceType, Pos>::type;

        template <typename SequenceType, typename Type>
        using push_back = typename TypeSeqPushBack<SequenceType, Type>::type;

        template <typename SequenceType, typename Type>
        using push_front = typename TypeSeqPushFront<SequenceType, Type>::type;

        template <typename SequenceType, int Pos>
        using remove = typename TypeSeqRemove<SequenceType, Pos>::type;

        template <typename SequenceType>
        using pop_front = typename TypeSeqPopFront<SequenceType>::type;

        template <typename SequenceType>
        using pop_back = typename TypeSeqPopBack<SequenceType>::type;

        template <typename SequenceType, template <typename> class TransformationTpl>
        using transform = typename TypeSeqTransform<SequenceType, TransformationTpl>::type;
    }
}
