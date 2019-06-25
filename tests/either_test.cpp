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