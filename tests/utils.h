#ifndef RVARAGO_KITTEN_TEST_UTILS_H
#define RVARAGO_KITTEN_TEST_UTILS_H

namespace rvarago::kitten::test::utils {

    template <typename T1, typename T2>
    inline constexpr bool is_same_after_decaying = std::is_same<std::decay_t<T1>, std::decay_t<T2>>::value;
}

#endif