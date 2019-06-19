#ifndef RVARAGO_KITTEN_VECTOR_H
#define RVARAGO_KITTEN_VECTOR_H

#include "kitten/detail/traits.h"
#include "kitten/functor.h"
#include "kitten/monad.h"
#include "kitten/ranges/algorithm.h"

namespace rvarago::kitten {

    template <template <typename...> typename SequenceContainer>
    struct monad<SequenceContainer> {

        template <typename UnaryFunction, typename A, typename... Rest, typename = detail::traits::enable_if_sequence_container<SequenceContainer<A, Rest...>>>
        static constexpr auto bind(SequenceContainer<A, Rest...> const& input, UnaryFunction f) -> decltype(f(std::declval<A>())) {
            auto mapped_sequence = decltype(f(std::declval<A>())){};
            for (auto const& el : input) {
                ranges::copy(f(el), std::back_inserter(mapped_sequence));
            }
            return mapped_sequence;
        }

    };

    template <template <typename...> typename SequenceContainer>
    struct functor<SequenceContainer> {

        template <typename UnaryFunction, typename A, typename... Rest, typename = detail::traits::enable_if_sequence_container<SequenceContainer<A, Rest...>>>
        static constexpr auto fmap(SequenceContainer<A, Rest...> const& input, UnaryFunction f) -> SequenceContainer<decltype(f(std::declval<A>()))> {
            return monad<SequenceContainer>::bind(input, [&f](auto const& v) { return SequenceContainer{f(v)}; });
        }

    };

}

#endif