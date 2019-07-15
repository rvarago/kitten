#ifndef RVARAGO_KITTEN_APPLICATIVE_H
#define RVARAGO_KITTEN_APPLICATIVE_H

#include <functional>
#include <tuple>

namespace rvarago::kitten {

    /**
     * An applicative is an abstraction that allows the combination of two wrapped values.
     *
     * Given two applicatives apa1: AP[A] and  apa2: AP[A] and a binary function f: A -> A -> B
     *  It uses f to map over apa1 and apa2 and then return a new applicative apb: AP[B].
     *
     */
    template <template <typename ...> typename AP, typename = void>
    struct applicative;

    namespace traits {
        template <template <typename ...> typename, typename = void>
        struct is_applicative : std::false_type{};

        template <template <typename ...> typename AP>
        inline constexpr bool is_applicative_v = is_applicative<AP>::value;
    }

    /**
     * Forwards to the applicative implementation.
     *
     * @param first an applicative apa1: AP[A]
     * @param second an applicative apa2: AP[A]
     * @param f a function A -> A -> B that maps over the values wrapped inside apa1 and apa2 to yield a value b: B
     * @return a new applicative apb: AP[B] resulting from applying f over the wrapped value inside apa1 and apa2
     */
    template <typename BinaryFunction, template <typename ...> typename AP, typename A, typename B, typename... Rest>
    constexpr decltype(auto) combine(AP<A, Rest...> const& first, AP<B, Rest...> const& second, BinaryFunction f) {
        static_assert(traits::is_applicative_v<AP>, "type constructor AP does not have an applicative instance");
        return applicative<AP>::combine(first, second, f);
    }


    /**
     * Infix version of combine. Since operator+ expects two arguments, we had to wrap the applicatives in a tuple.
     */
    template <typename BinaryFunction, template <typename ...> typename AP, typename A, typename B, typename... Rest>
    constexpr decltype(auto) operator+(std::tuple<AP<A, Rest...>, AP<B, Rest...>> const& input, BinaryFunction f) {
        return combine(std::get<0>(input), std::get<1>(input), f);
    }

    /**
     * Infix version of combine that receives unwrapped applicatives and uses + as a binary function.
     */
    template <template <typename ...> typename AP, typename A, typename B, typename... Rest>
    constexpr decltype(auto) operator+(AP<A, Rest...> const& first, AP<B, Rest...> const& second) {
        return combine(first, second, std::plus{});
    }

}

#endif
