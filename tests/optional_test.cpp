#include <gtest/gtest.h>

#include <string>
#include <kitten/instances/optional.h>

#include "utils.h"

using namespace rvarago::kitten;
using test::utils::is_same_after_decaying;

namespace {

    TEST(optional, fmap_should_returnEmpty_when_empty) {
        auto const none = std::optional<int>{};
        auto const mapped_none = none | [](auto v){ return std::to_string(v * 10); };

        static_assert(is_same_after_decaying<std::optional<std::string>, decltype(mapped_none)>);

        EXPECT_TRUE(!mapped_none.has_value());
    }

    TEST(optional, fmap_should_returnMapped_when_notEmpty) {
        auto const some_one = std::optional<int>{1};
        auto const mapped_some = some_one | [](auto v){ return std::to_string(v * 10); };

        static_assert(is_same_after_decaying<std::optional<std::string>, decltype(mapped_some)>);

        EXPECT_TRUE(mapped_some.has_value());
        EXPECT_EQ("10", mapped_some.value());
    }

    TEST(optional, bind_should_returnEmpty_when_empty) {
        auto const none = std::optional<int>{};
        auto const mapped_none = none >> [](auto v){ return std::optional{std::to_string(v * 10)}; };

        static_assert(is_same_after_decaying<std::optional<std::string>, decltype(mapped_none)>);

        EXPECT_TRUE(!mapped_none.has_value());
    }

    TEST(optional, bind_should_returnMapped_when_notEmpty) {
        auto const some_one = std::optional<int>{1};
        auto const mapped_some = some_one >> [](auto v){ return std::optional{std::to_string(v * 10)}; };

        static_assert(is_same_after_decaying<std::optional<std::string>, decltype(mapped_some)>);

        EXPECT_TRUE(mapped_some.has_value());
        EXPECT_EQ("10", mapped_some.value());
    }

}