#ifndef RVARAGO_KITTEN_SEQUENCE_CONTAINER_H
#define RVARAGO_KITTEN_SEQUENCE_CONTAINER_H

#include "kitten/detail/traits.h"
#include "kitten/applicative.h"
#include "kitten/functor.h"
#include "kitten/monad.h"
#include "kitten/ranges/algorithm.h"

namespace rvarago::kitten {

    template <template <typename...> typename SequenceContainer>
    struct monad<SequenceContainer> {

        template <typename UnaryFunction, typename A, typename... Rest, typename = detail::traits::enable_if_sequence_container<SequenceContainer>>
        static constexpr auto bind(SequenceContainer<A, Rest...> const& input, UnaryFunction f) -> decltype(f(std::declval<A>())) {
            auto mapped_sequence = decltype(f(std::declval<A>())){};
            for (auto const& el : input) {
                ranges::copy(f(el), std::back_inserter(mapped_sequence));
            }
            return mapped_sequence;
        }

    };

    template <template <typename...> typename SequenceContainer>
    struct applicative<SequenceContainer> {

        template <typename BinaryFunction, typename A, typename B, typename... Rest, typename = detail::traits::enable_if_sequence_container<SequenceContainer>>
        static constexpr auto combine(SequenceContainer<A, Rest...> const &first, SequenceContainer<B, Rest...> const& second, BinaryFunction f) -> SequenceContainer<decltype(f(std::declval<A>(), std::declval<B>()))> {
            using ValueT = decltype(f(std::declval<A>(), std::declval<B>()));
            auto combined_sequence = SequenceContainer<ValueT>{};
            for (auto const& first_element : first) {
                for (auto const& second_element: second) {
                    combined_sequence.push_back(f(first_element, second_element));
                }
            }
            return combined_sequence;
        }

    };

    template <template <typename...> typename SequenceContainer>
    struct functor<SequenceContainer> {

        template <typename UnaryFunction, typename A, typename... Rest, typename = detail::traits::enable_if_sequence_container<SequenceContainer>>
        static constexpr auto map(SequenceContainer<A, Rest...> const& input, UnaryFunction f) -> SequenceContainer<decltype(f(std::declval<A>()))> {
            return monad<SequenceContainer>::bind(input, [&f](auto const& v) { return SequenceContainer{f(v)}; });
        }

    };

    namespace detail {
        template <template <typename...> typename SequenceContainer>
        struct is_monad<SequenceContainer> : std::true_type{};

        template <template <typename...> typename SequenceContainer>
        struct is_applicative<SequenceContainer> : std::true_type{};

        template <template <typename...> typename SequenceContainer>
        struct is_functor<SequenceContainer> : std::true_type{};
    }

}

#endif