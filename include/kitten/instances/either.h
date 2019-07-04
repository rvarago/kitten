#ifndef RVARAGO_KITTEN_EITHER_H
#define RVARAGO_KITTEN_EITHER_H

#include <variant>

#include "kitten/applicative.h"
#include "kitten/functor.h"
#include "kitten/monad.h"

namespace rvarago::kitten {

    namespace types {
        template<typename A, typename E>
        using either = std::variant<A, E>;
    }

    template <>
    struct monad<std::variant> {

        template <typename UnaryFunction, typename A, typename E>
        static constexpr auto bind(std::variant<A, E> const &input, UnaryFunction f) -> decltype(f(std::declval<A>())) {
            using ResultT = decltype(f(std::declval<A>()));
            if (std::holds_alternative<E>(input)) {
                return ResultT{std::get<E>(input)};
            }
            return f(std::get<A>(input));
        }

    };

    template <>
    struct applicative<std::variant> {

        template <typename BinaryFunction, typename A, typename B, typename E>
        static constexpr auto combine(std::variant<A, E> const &first, std::variant<B, E> const& second, BinaryFunction f) -> std::variant<decltype(f(std::declval<A>(), std::declval<B>())), E> {
            if (std::holds_alternative<A>(first) && std::holds_alternative<B>(second)) {
                return f(std::get<A>(first), std::get<B>(second));
            }

            if (std::holds_alternative<E>(first)) {
                return std::get<E>(first);
            }

            return std::get<E>(second);
        }

    };

    template <>
    struct functor<std::variant> {

        template <typename UnaryFunction, typename A, typename E>
        static constexpr auto map(std::variant<A, E> const &input, UnaryFunction f) -> std::variant<decltype(f(std::declval<A>())), E> {
            return monad<std::variant>::bind(input, [&f](auto const& value){ return std::variant<decltype(f(std::declval<A>())), E>{f(value)}; });
        }

    };

    namespace detail {
        template <>
        struct is_monad<std::variant> : std::true_type{};

        template <>
        struct is_applicative<std::variant> : std::true_type{};

        template <>
        struct is_functor<std::variant> : std::true_type{};
    }

}

#endif
