#ifndef RVARAGO_KITTEN_MONAD_H
#define RVARAGO_KITTEN_MONAD_H

namespace rvarago::kitten {

    /**
    * A monad is an abstraction that allows to be mapped over where the mapping function itself returns a monad.
    *
    * Given a monad ma: M[A] and a function f: A -> M[B]
    *  It uses f to map over over ma, flat the return type, and then return a new monad mb: M[B].
    *
    * Note that if we had done as we do for functors, i.e. to do fmap(ma, f), we'd have a M[M[B]] that should then be flattened.
    * but bind does both the mapping and then the flattening.
    *
    * Laws:
    *
    * - Left identity: return a >>= f == f a
    * - Right identity: m >>= return == m
    * - Associativity: (m >>= f) >>= g == m >>= (\x -> f x >>= g)
    */
    template <template <typename ...> typename M, typename Enable = void>
    struct monad;

    /**
     * Forwards to the monad implementation.
     *
     * @param input a monad ma: M[A]
     * @param f a function A -> M[B] that maps over the value wrapped inside ma to produce a new monad mb_temp: M[B]
     * @return a new monad mb: M[B] resulting from applying f over the wrapped value inside ma and then flattening the result
     */
    template <typename UnaryFunction, template <typename ...> typename M, typename A, typename... Rest>
    decltype(auto) bind(M<A, Rest...> const& input, UnaryFunction f) {
        return monad<M>::bind(input, f);
    }

    /**
     * Infix version of bind.
     */
    template <typename UnaryFunction, template <typename ...> typename M, typename A, typename... Rest>
    decltype(auto) operator>>(M<A, Rest...> const& input, UnaryFunction f) {
        return bind(input, f);
    }

}

#endif
