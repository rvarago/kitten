#include <gtest/gtest.h>

#include <string>
#include <kitten/instances/optional.h>

#include "utils.h"

namespace {

    using namespace rvarago::kitten;
    using test::utils::is_same_after_decaying;

    TEST(optional, map_should_returnEmpty_when_empty) {
        auto const none = std::optional<int>{};
        auto const mapped_none = none | [](auto v){ return std::to_string(v * 10); };

        static_assert(is_same_after_decaying<std::optional<std::string>, decltype(mapped_none)>);

        EXPECT_TRUE(!mapped_none.has_value());
    }

    TEST(optional, map_should_returnMapped_when_notEmpty) {
        auto const some_one = std::optional<int>{1};
        auto const mapped_some = some_one | [](auto v){ return std::to_string(v * 10); };

        static_assert(is_same_after_decaying<std::optional<std::string>, decltype(mapped_some)>);

        EXPECT_TRUE(mapped_some.has_value());
        EXPECT_EQ("10", mapped_some.value());
    }

    TEST(optional, combine_should_returnEmpty_when_empty) {
        auto const none = std::optional<int>{};
        auto const some_three= std::optional<int>{3};

        auto const sum = none + some_three;
        auto const product_as_string = std::tuple{some_three, none} + [](auto const& first, auto const& second) {
            return std::to_string(first * second);
        };

        static_assert(is_same_after_decaying<std::optional<int>, decltype(sum)>);
        static_assert(is_same_after_decaying<std::optional<std::string>, decltype(product_as_string)>);

        EXPECT_TRUE(!sum.has_value());
        EXPECT_TRUE(!product_as_string.has_value());
    }

    TEST(optional, combine_should_returnCombined_when_notEmpty) {
        auto const some_two = std::optional<int>{2};
        auto const some_three = std::optional<int>{3};

        auto const sum = some_two + some_three;
        auto const product_as_string = std::tuple{some_three, some_two} + [](auto const& first, auto const& second) {
            return std::to_string(first * second);
        };

        static_assert(is_same_after_decaying<std::optional<int>, decltype(sum)>);
        static_assert(is_same_after_decaying<std::optional<std::string>, decltype(product_as_string)>);

        EXPECT_TRUE(sum.has_value());
        EXPECT_EQ(5, sum.value());
        EXPECT_TRUE(product_as_string.has_value());
        EXPECT_EQ("6", product_as_string.value());
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