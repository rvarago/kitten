#ifndef RVARAGO_KITTEN_EITHER_H
#define RVARAGO_KITTEN_EITHER_H

#include <variant>

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
    struct functor<std::variant> {

        template <typename UnaryFunction, typename A, typename E>
        static constexpr auto map(std::variant<A, E> const &input, UnaryFunction f) -> std::variant<decltype(f(std::declval<A>())), E> {
            return monad<std::variant>::bind(input, [&f](auto const& value){ return std::variant<decltype(f(std::declval<A>())), E>{f(value)}; });
        }

    };

}

#endif
