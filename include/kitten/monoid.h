#ifndef RVARAGO_KITTEN_MONOID_H
#define RVARAGO_KITTEN_MONOID_H

#include <numeric>
#include <type_traits>

namespace rvarago::kitten {

/**
 * A monoid is an abstraction that allows to append elements via an associative binary operation, and provides an
 * identity, or neutral, element.
 *
 * Given the monoids a and b: M and a function f: M[A] -> M[A] -> M[A]
 *  It uses f to append a to b yielding c of type M[A].
 *
 * Laws:
 *
 * - Associativity: (a mappend b) mappend c = a mappend (b mappend c) = a mappend b mappend c
 * - Identity: mempty mappend a = a mappend mempty = a
 */
template <template <typename...> typename M, typename = void>
struct monoid;

namespace traits {
template <template <typename...> typename, typename = void>
struct is_monoid : std::false_type {};

template <template <typename...> typename M>
inline constexpr bool is_monoid_v = is_monoid<M>::value;
}

/**
 * Returns the identity element for the given monoid instance.
 *
 * @return the identity element for the monoid M
 */
template <template <typename...> typename M, typename A>
constexpr decltype(auto) mempty() {
    static_assert(traits::is_monoid_v<M>, "type constructor M does not have a monoid instance");
    return monoid<M>::template mempty<A>();
}

/**
 * Given two monoids ma1: M[A] and ma2: M[A], uses the binary function f to combine both, yielding another
 * monoid of type M[A].
 *
 * @param first a monoid: M[A]
 * @param second a monoid: M[A]
 * @param f a function M[A] -> M[A] -> M[A] that combines the monoids first and second returning a new monoid
 * @return a new monoid: M[A] resulting from applying f over first and second
 */
template <template <typename...> typename M, typename A, typename BinaryFunction = std::plus<>>
constexpr decltype(auto) mappend(M<A> const &first, M<A> const &second, BinaryFunction f = BinaryFunction{}) {
    static_assert(traits::is_monoid_v<M>, "type constructor M does not have a monoid instance");
    return monoid<M>::mappend(first, second, f);
}

}

#endif
