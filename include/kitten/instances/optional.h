#ifndef RVARAGO_KITTEN_OPTIONAL_H
#define RVARAGO_KITTEN_OPTIONAL_H

#include <optional>

#include "kitten/applicative.h"
#include "kitten/functor.h"
#include "kitten/monad.h"

#include "kitten/detail/deriving/from_monad/derive_applicative.h"
#include "kitten/detail/deriving/from_monad/derive_functor.h"

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
            return detail::deriving::combine(first, second, f);
        }

        template <typename A>
        static constexpr auto pure(A&& value) -> std::optional<A> {
            return detail::deriving::pure<std::optional>(std::forward<A>(value));
        }

    };

    template <>
    struct functor<std::optional> {

        template <typename UnaryFunction, typename A>
        static constexpr auto fmap(std::optional<A> const &input, UnaryFunction f) -> std::optional<decltype(f(std::declval<A>()))> {
            return detail::deriving::fmap(input, f);
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
