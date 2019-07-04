#ifndef RVARAGO_KITTEN_MULTIFUNCTOR_H
#define RVARAGO_KITTEN_MULTIFUNCTOR_H

namespace rvarago::kitten {

    /**
     * A multifunctor is an abstraction that allows to be mapped over for multiple types "in parallel".
     *
     * Given a multifunctor fi: F[A1, ..., Z1] and a function f: A1, ..., Z1 -> A2, ..., Z2
     *  It uses f to map over over fa and then return a new functor fb: F[A2, ..., Z2].
     */
    template <template <typename ...> typename MF, typename = void>
    struct multifunctor;

    namespace detail {
        template <template <typename ...> typename, typename = void>
        struct is_multifunctor : std::false_type{};

        template <template <typename ...> typename MF>
        inline constexpr bool is_multifunctor_v = is_multifunctor<MF>::value;
    }

    /**
     * Forwards to the multifunctor implementation.
     *
     * @param input a n-functor fa: F[A1, ..., Z1]
     * @param f a function A1, ..., Z1 -> A2, ..., Z2 that maps over the value wrapped inside fa to yield a value b: A2, ..., Z2
     * @return a new multifunctor fb: F[A2, ..., Z2] resulting from applying f over the wrapped value inside fa
     */
    template <typename UnaryFunction, template <typename ...> typename MF, typename... Rest>
    constexpr decltype(auto) multimap(MF<Rest...> const& input, UnaryFunction f) {
        static_assert(detail::is_multifunctor_v<MF>, "type constructor MF does not have a multifunctor instance");
        return multifunctor<MF>::multimap(input, f);
    }

    /**
     * Infix version of multimap.
     */
    template <typename UnaryFunction, template <typename ...> typename MF, typename A, typename... Rest>
    constexpr decltype(auto) operator||(MF<A, Rest...> const& input, UnaryFunction f) {
        return multimap(input, f);
    }
}

#endif
