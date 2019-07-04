#ifndef RVARAGO_KITTEN_OPTIONAL_H
#define RVARAGO_KITTEN_OPTIONAL_H

#include <optional>

#include "kitten/applicative.h"
#include "kitten/functor.h"
#include "kitten/monad.h"

namespace rvarago::kitten {

    template <>
    struct monad<std::optional> {

        template <typename UnaryFunction, typename A>
        static constexpr auto bind(std::optional<A> const &input, UnaryFunction f) -> decltype(f(std::declval<A>())) {
            if (!input.has_value()) {
                return {};
            }
            return f(input.value());
        }

    };

    template <>
    struct applicative<std::optional> {

        template <typename BinaryFunction, typename A, typename B>
        static constexpr auto combine(std::optional<A> const &first, std::optional<B> const& second, BinaryFunction f) -> std::optional<decltype(f(std::declval<A>(), std::declval<B>()))> {
            if (!first.has_value() || !second.has_value()) {
                return {};
            }
            return f(first.value(), second.value());
        }

    };

    template <>
    struct functor<std::optional> {

        template <typename UnaryFunction, typename A>
        static constexpr auto map(std::optional<A> const &input, UnaryFunction f) -> std::optional<decltype(f(std::declval<A>()))> {
            return monad<std::optional>::bind(input, [&f](auto const& value){ return std::optional{f(value)}; });
        }

    };

    namespace detail {
        template <>
        struct is_monad<std::optional> : std::true_type{};
    }

}

#endif
