#ifndef RVARAGO_KITTEN_SEQUENCE_CONTAINER_H
#define RVARAGO_KITTEN_SEQUENCE_CONTAINER_H

#include <deque>
#include <list>
#include <type_traits>
#include <vector>

#include "kitten/applicative.h"
#include "kitten/functor.h"
#include "kitten/monad.h"

#include "kitten/detail/deriving/from_monad/derive_applicative.h"
#include "kitten/detail/deriving/from_monad/derive_functor.h"

#include "kitten/detail/ranges/algorithm.h"

namespace rvarago::kitten {

// TODO: Use standard concepts
namespace detail {

template <template <typename...> typename Container>
struct is_sequence_container : std::false_type {};

template <>
struct is_sequence_container<std::deque> : std::true_type {};

template <>
struct is_sequence_container<std::list> : std::true_type {};

template <>
struct is_sequence_container<std::vector> : std::true_type {};

template <template <typename...> typename Container>
using enable_if_sequence_container = typename std::enable_if_t<is_sequence_container<Container>::value>;

}

template <template <typename...> typename SequenceContainer>
struct monad<SequenceContainer> {

    template <typename A, typename UnaryFunction, typename = detail::enable_if_sequence_container<SequenceContainer>>
    static constexpr auto bind(SequenceContainer<A> const &input, UnaryFunction f) -> decltype(f(std::declval<A>())) {
        auto mapped_sequence = decltype(f(std::declval<A>())){};
        for (auto const &el : input) {
            detail::ranges::copy(f(el), std::back_inserter(mapped_sequence));
        }
        return mapped_sequence;
    }

    template <typename A, typename = detail::enable_if_sequence_container<SequenceContainer>>
    static constexpr auto wrap(A &&value) -> SequenceContainer<A> {
        return SequenceContainer<A>{std::forward<A>(value)};
    }
};

template <template <typename...> typename SequenceContainer>
struct applicative<SequenceContainer> {

    template <typename A, typename B, typename BinaryFunction,
              typename = detail::enable_if_sequence_container<SequenceContainer>>
    static constexpr auto combine(SequenceContainer<A> const &first, SequenceContainer<B> const &second,
                                  BinaryFunction f)
        -> SequenceContainer<decltype(f(std::declval<A>(), std::declval<B>()))> {
        return detail::deriving::combine(first, second, f);
    }

    template <typename A, typename = detail::enable_if_sequence_container<SequenceContainer>>
    static constexpr auto pure(A &&value) -> SequenceContainer<A> {
        return detail::deriving::pure<SequenceContainer>(std::forward<A>(value));
    }
};

template <template <typename...> typename SequenceContainer>
struct functor<SequenceContainer> {

    template <typename A, typename UnaryFunction, typename = detail::enable_if_sequence_container<SequenceContainer>>
    static constexpr auto fmap(SequenceContainer<A> const &input, UnaryFunction f)
        -> SequenceContainer<decltype(f(std::declval<A>()))> {
        return detail::deriving::fmap(input, f);
    }
};

namespace traits {
template <template <typename...> typename SequenceContainer>
struct is_monad<SequenceContainer> : std::true_type {};

template <template <typename...> typename SequenceContainer>
struct is_applicative<SequenceContainer> : std::true_type {};

template <template <typename...> typename SequenceContainer>
struct is_functor<SequenceContainer> : std::true_type {};
}

}

#endif