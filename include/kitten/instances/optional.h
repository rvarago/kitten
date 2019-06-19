#ifndef RVARAGO_KITTEN_OPTIONAL_H
#define RVARAGO_KITTEN_OPTIONAL_H

#include <optional>

#include "kitten/functor.h"
#include "kitten/monad.h"

namespace rvarago::kitten {

    template <>
    struct monad<std::optional> {

        template <typename UnaryFunction, typename A>
        static constexpr auto bind(std::optional <A> const &input, UnaryFunction f) -> decltype(f(std::declval<A>())) {
            if (!input.has_value()) {
                return {};
            }
            return f(input.value());
        }

    };

    template <>
    struct functor<std::optional> {

        template <typename UnaryFunction, typename A>
        static constexpr auto fmap(std::optional <A> const &input, UnaryFunction f) -> std::optional<decltype(f(std::declval<A>()))> {
            return monad<std::optional>::bind(input, [&f](auto const& value){ return std::optional{f(value)}; });
        }

    };

}

#endif
