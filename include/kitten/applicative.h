#ifndef RVARAGO_KITTEN_APPLICATIVE_H
#define RVARAGO_KITTEN_APPLICATIVE_H

#include <functional>
#include <tuple>

namespace rvarago::kitten {

/**
 * An applicative is an abstraction that allows the combination of two wrapped values.
 *
 * Given two applicatives apa: AP[A] and  apb: AP[B] and a binary function f: (A, B) -> C
 *  It uses f to map over the values unwrapped from apa and apb and then returns a new applicative apc: AP[C].
 *
 */
template <template <typename...> typename AP, typename = void>
struct applicative;

namespace traits {
template <template <typename...> typename, typename = void>
struct is_applicative : std::false_type {};

template <template <typename...> typename AP>
inline constexpr bool is_applicative_v = is_applicative<AP>::value;
}

/**
 * Lifts a value of type A into an applicative AP[A].
 *
 * @param value the type parameter that defines the type of the element wrapped by the applicative
 * @param tail eventual remaining type parameters used by the applicative, considered as implementation detail
 * @return a new monad m: AP[A] that wraps the contained value of type A
 */
template <template <typename...> typename AP, typename A>
constexpr decltype(auto) pure(A &&value) {
    static_assert(traits::is_applicative_v<AP>, "type constructor M does not have an applicative instance");
    return applicative<AP>::pure(std::forward<A>(value));
}

/**
 * Unwraps the applicatives apa: AP[A] and apb: AP[B], feeds the unwrapped value of types A and B into the function
 * f: (A, B) -> C, and then returns its result wrapped in a functor F[B]..
 *
 * @param first an applicative apa: AP[A]
 * @param second an applicative apb: AP[B]
 * @param f a function A -> B -> C that maps over the values unwrapped from apa and apb to yield a value c: C
 * @return a new applicative apc: AP[C] resulting from wrapping the application of f over the wrapped value inside apa
 * and apb
 */
template <template <typename...> typename AP, typename A, typename B, typename BinaryFunction = std::plus<>>
constexpr decltype(auto) combine(AP<A> const &first, AP<B> const &second, BinaryFunction f = BinaryFunction{}) {
    static_assert(traits::is_applicative_v<AP>, "type constructor AP does not have an applicative instance");
    return applicative<AP>::combine(first, second, f);
}

/**
 * Infix version of combine. Since operator+ expects two arguments, we had to wrap the applicatives in a tuple.
 */
template <template <typename...> typename AP, typename A, typename B, typename BinaryFunction>
constexpr decltype(auto) operator+(std::tuple<AP<A>, AP<B>> const &input, BinaryFunction f) {
    return combine(std::get<0>(input), std::get<1>(input), f);
}

/**
 * Infix version of combine that receives unwrapped applicatives and uses + as a binary function.
 */
template <template <typename...> typename AP, typename A, typename B>
constexpr decltype(auto) operator+(AP<A> const &first, AP<B> const &second) {
    return combine(first, second);
}

/**
 * lifts a binary function f: A -> B -> C into an applicative context fap: AP[A] -> AP[B] -> AP[C].
 *
 * @param f binary function f: A -> B -> C to be lifted into the applicative AP[T] context.
 * @return the lifted version of f that operates on applicatives.
 */
template <template <typename...> typename AP, typename BinaryFunction>
constexpr decltype(auto) liftA2(BinaryFunction f) {
    return [f](auto const &first, auto const &second) { return combine<AP>(first, second, f); };
}

}

#endif
