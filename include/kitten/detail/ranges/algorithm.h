#ifndef RVARAGO_KITTEN_ALGORITHM_H
#define RVARAGO_KITTEN_ALGORITHM_H

#include <algorithm>

// TODO: Use standard ranges
namespace rvarago::kitten::detail::ranges {

template <typename Range, typename OutIterator>
constexpr decltype(auto) copy(Range &&range, OutIterator out) {
    return std::copy(std::cbegin(range), std::cend(range), out);
}

template <typename Range, typename UnaryFunction>
constexpr decltype(auto) for_each(Range &&range, UnaryFunction f) {
    return std::for_each(std::cbegin(range), std::cend(range), f);
}

}

#endif