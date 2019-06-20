#ifndef RVARAGO_KITTEN_TRAITS_H
#define RVARAGO_KITTEN_TRAITS_H

#include <deque>
#include <list>
#include <vector>

namespace rvarago::kitten::detail::traits {

    // TODO: Use standard concepts
    template <typename Container>
    struct is_sequence_container final : std::false_type {};

    template <typename T>
    struct is_sequence_container<std::deque<T>> final : std::true_type {};

    template <typename T>
    struct is_sequence_container<std::list<T>> final : std::true_type {};

    template <typename T>
    struct is_sequence_container<std::vector<T>> final : std::true_type {};

    template <typename Container>
    using enable_if_sequence_container = typename std::enable_if_t<traits::is_sequence_container<Container>::value>;

}

#endif