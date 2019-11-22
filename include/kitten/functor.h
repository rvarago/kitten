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
 * - Identity: fmap (f . g)  ==  fmap f . fmap g
 * - Composition: fmap id = id
 */
template <template <typename...> typename F, typename = void>
struct functor;

namespace traits {
template <template <typename...> typename, typename = void>
struct is_functor : std::false_type {};

template <template <typename...> typename F>
inline constexpr bool is_functor_v = is_functor<F>::value;
}

/**
 * Forwards to the functor implementation.
 *
 * @param input a functor fa: F[A]
 * @param f a function A -> B that maps over the value wrapped inside fa to yield a value b: B
 * @return a new functor fb: F[B] resulting from applying f over the wrapped value inside fa
 */
template <template <typename...> typename F, typename A, typename UnaryFunction>
constexpr decltype(auto) fmap(F<A> const &input, UnaryFunction f) {
    static_assert(traits::is_functor_v<F>, "type constructor F does not have a functor instance");
    return functor<F>::fmap(input, f);
}

/**
 * Infix version of fmap.
 */
template <template <typename...> typename F, typename A, typename UnaryFunction>
constexpr decltype(auto) operator|(F<A> const &input, UnaryFunction f) {
    return fmap(input, f);
}

/**
 * Lifts a function A -> B into a function F[A] -> F[B], where F[_] is a functor.
 *
 * @param f a function A -> B that shall be applied to the content of F[A] where it's later provided
 * @return a lifted function F[A] -> F[B]
 */
template <template <typename...> typename F, typename UnaryFunction>
constexpr decltype(auto) liftF(UnaryFunction f) {
    return [f](auto const &input) { return functor<F>::fmap(input, f); };
}

}

#endif
