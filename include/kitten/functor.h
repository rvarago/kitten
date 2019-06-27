#ifndef RVARAGO_KITTEN_FUNCTOR_H
#define RVARAGO_KITTEN_FUNCTOR_H

namespace rvarago::kitten {

    /**
     * A functor is an abstraction that allows to be mapped over.
     *
     * Given a functor fa: F[A] and a function f: A -> B
     *  It uses f to map over fa and then return a new functor fb: F[B].
     *
     * Laws:
     *
     * - Identity: map (f . g)  ==  map f . map g
     * - Composition: map id = id
     */
    template <template <typename ...> typename F, typename Enable = void>
    struct functor;

    /**
     * Forwards to the functor implementation.
     *
     * @param input a functor fa: F[A]
     * @param f a function A -> B that maps over the value wrapped inside fa to yield a value b: B
     * @return a new functor fb: F[B] resulting from applying f over the wrapped value inside fa
     */
    template <typename UnaryFunction, template <typename ...> typename F, typename A, typename... Rest>
    constexpr decltype(auto) map(F<A, Rest...> const& input, UnaryFunction f) {
        return functor<F>::map(input, f);
    }

    /**
     * Infix version of map.
     */
    template <typename UnaryFunction, template <typename ...> typename F, typename A, typename... Rest>
    constexpr decltype(auto) operator|(F<A, Rest...> const& input, UnaryFunction f) {
        return map(input, f);
    }
}

#endif
