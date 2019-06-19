#ifndef RVARAGO_KITTEN_VARIANT_H
#define RVARAGO_KITTEN_VARIANT_H

#include <variant>

#include "kitten/multifunctor.h"

namespace rvarago::kitten {

    namespace syntax {

        template <typename... Rest>
        struct overloaded final : Rest... {
            using Rest::operator()...;
        };

        template <typename... Rest> overloaded(Rest...) -> overloaded<Rest...>;

    }

    template <>
    struct multifunctor<std::variant> {

        template <typename UnaryFunction, typename... Rest>
        static constexpr auto nfmap(std::variant<Rest...> const &input, UnaryFunction f) -> std::variant<decltype(f(std::declval<Rest>()))...> {
            using ResultT = std::variant<decltype(f(std::declval<Rest>()))...>;
            return std::visit([&f](auto const& value){ return ResultT{f(value)}; }, input);
        }

    };

}

#endif
