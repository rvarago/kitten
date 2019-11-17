#ifndef RVARAGO_KITTEN_ALGORITHM_H
#define RVARAGO_KITTEN_ALGORITHM_H

#include <algorithm>

namespace rvarago::kitten::detail::ranges {

    // TODO: Use standard ranges
    template <typename Range, typename OutIterator>
    decltype(auto) copy(Range&& range, OutIterator out) {
        return std::copy(std::cbegin(range), std::cend(range), out);
    }

}

#endif