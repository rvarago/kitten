#include <gtest/gtest.h>

#include <string>
#include <kitten/instances/either.h>

#include "utils.h"

using namespace rvarago::kitten;
using test::utils::is_same_after_decaying;

namespace {

    struct error_t final {
        explicit error_t(int _code) : code{_code} {}
        int const code;
    };

    TEST(either, map_should_returnEmpty_when_empty) {
        auto const error = types::either<int, error_t>{error_t{-1}};
        auto const mapped_error = error | [](auto v){ return std::to_string(v * 10); };

        static_assert(is_same_after_decaying<types::either<std::string, error_t>, decltype(mapped_error)>);

        EXPECT_TRUE(std::holds_alternative<error_t>(mapped_error));
        EXPECT_EQ(-1, std::get<error_t>(mapped_error).code);
    }

    TEST(either, map_should_returnMapped_when_notEmpty) {
        auto const value_one = types::either<int, error_t>{1};
        auto const mapped_value = value_one | [](auto v){ return std::to_string(v * 10); };

        static_assert(is_same_after_decaying<types::either<std::string, error_t>, decltype(mapped_value)>);

        EXPECT_TRUE(std::holds_alternative<std::string>(mapped_value));
        EXPECT_EQ("10", std::get<std::string>(mapped_value));
    }

    TEST(either, combine_should_returnEmpty_when_empty) {
        auto const error = types::either<int, error_t>{error_t{-1}};
        auto const some_three = types::either<int, error_t>{3};

        auto const sum = error + some_three;
        auto const product_as_string = std::tuple{some_three, error} + [](auto const& first, auto const& second) {
            return std::to_string(first * second);
        };

        static_assert(is_same_after_decaying<types::either<int, error_t>, decltype(sum)>);
        static_assert(is_same_after_decaying<types::either<std::string, error_t>, decltype(product_as_string)>);

        EXPECT_TRUE(std::holds_alternative<error_t>(sum));
        EXPECT_EQ(-1, std::get<error_t>(sum).code);

        EXPECT_TRUE(std::holds_alternative<error_t>(product_as_string));
        EXPECT_EQ(-1, std::get<error_t>(product_as_string).code);
    }

    TEST(either, combine_should_returnCombined_when_notEmpty) {
        auto const some_two = types::either<int, error_t>{2};
        auto const some_three = types::either<int, error_t>{3};

        auto const sum = some_two + some_three;
        auto const product_as_string = std::tuple{some_three, some_two} + [](auto const& first, auto const& second) {
            return std::to_string(first * second);
        };

        static_assert(is_same_after_decaying<types::either<int, error_t>, decltype(sum)>);
        static_assert(is_same_after_decaying<types::either<std::string, error_t>, decltype(product_as_string)>);


        EXPECT_TRUE(std::holds_alternative<int>(sum));
        EXPECT_EQ(5, std::get<int>(sum));

        EXPECT_TRUE(std::holds_alternative<std::string>(product_as_string));
        EXPECT_EQ("6", std::get<std::string>(product_as_string));
    }

    TEST(either, bind_should_returnEmpty_when_empty) {
        auto const error = types::either<int, error_t>{error_t{-1}};
        auto const mapped_error = error >> [](auto v){ return types::either<std::string, error_t>{std::to_string(v * 10)}; };

        static_assert(is_same_after_decaying<types::either<std::string, error_t>, decltype(mapped_error)>);

        EXPECT_TRUE(std::holds_alternative<error_t>(mapped_error));
        EXPECT_EQ(-1, std::get<error_t>(mapped_error).code);
    }

    TEST(either, bind_should_returnMapped_when_notEmpty) {
        auto const value_one = types::either<int, error_t>{1};
        auto const mapped_value = value_one >> [](auto v){ return types::either<std::string, error_t>{std::to_string(v * 10)}; };

        static_assert(is_same_after_decaying<types::either<std::string, error_t>, decltype(mapped_value)>);

        EXPECT_TRUE(std::holds_alternative<std::string>(mapped_value));
        EXPECT_EQ("10", std::get<std::string>(mapped_value));
    }

}