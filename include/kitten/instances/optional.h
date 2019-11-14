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
            if (!input) {
                return std::nullopt;
            }
            return f(*input);
        }

        template <typename A>
        static constexpr auto wrap(A&& value) -> std::optional<A> {
            return std::make_optional(std::forward<A>(value));
        }

    };

    template <>
    struct applicative<std::optional> {

        template <typename BinaryFunction, typename A, typename B>
        static constexpr auto combine(std::optional<A> const &first, std::optional<B> const& second, BinaryFunction f) -> std::optional<decltype(f(std::declval<A>(), std::declval<B>()))> {
            using MonadT = monad<std::optional>;
            return MonadT::bind(first, [&second, &f](auto const& first_value) {
                return MonadT::bind(second, [&first_value, &f](auto const& second_value) {
                    return MonadT::wrap(f(first_value, second_value));
                });
            });
        }

    };

    template <>
    struct functor<std::optional> {

        template <typename UnaryFunction, typename A>
        static constexpr auto fmap(std::optional<A> const &input, UnaryFunction f) -> std::optional<decltype(f(std::declval<A>()))> {
            using MonadT = monad<std::optional>;
            return MonadT::bind(input, [&f](auto const& value){ return MonadT::wrap(f(value)); });
        }

    };

    namespace traits {
        template <>
        struct is_monad<std::optional> : std::true_type{};

        template <>
        struct is_applicative<std::optional> : std::true_type{};

        template <>
        struct is_functor<std::optional> : std::true_type{};
    }

}

#endif
