#ifndef RVARAGO_KITTEN_TRAITS_H
#define RVARAGO_KITTEN_TRAITS_H

#include <deque>
#include <list>
#include <vector>

namespace rvarago::kitten::detail::traits {

    // TODO: Use standard concepts
    template <template <typename ...> typename Container>
    struct is_sequence_container : std::false_type {};

    template <>
    struct is_sequence_container<std::deque> : std::true_type {};

    template <>
    struct is_sequence_container<std::list> : std::true_type {};

    template <>
    struct is_sequence_container<std::vector> : std::true_type {};

    template <template <typename ...> typename Container>
    using enable_if_sequence_container = typename std::enable_if_t<traits::is_sequence_container<Container>::value>;

}

#endif