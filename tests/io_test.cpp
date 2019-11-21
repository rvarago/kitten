#include <gtest/gtest.h>

#include <iostream>

#include <kitten/instances/io.h>
#include <string>

#include "utils.h"

namespace {

using namespace rvarago::kitten;
using test::utils::is_same_after_decaying;

TEST(io, map_should_returnMapped_when_notEmpty) {
  auto const print_hello = types::IO{[] { return 1; }};
  auto print_world = [](auto v) { return v + 1; };

  auto print_hello_world = print_hello | print_world;
  EXPECT_EQ(2, print_hello_world.run());
}

//    TEST(io, pure_should_returnANonEmptyApplicative) {
//        auto const some_one = pure<std::io>(1);
//
//        static_assert(is_same_after_decaying<std::io<int>,
//        decltype(some_one)>);
//
//        EXPECT_TRUE(some_one.has_value());
//        EXPECT_EQ(1, some_one.value());
//    }
//
//    TEST(io, combine_should_returnEmpty_when_empty) {
//        auto const none = std::io<int>{};
//        auto const some_three= std::io<int>{3};
//
//        auto const sum = none + some_three;
//        auto const product_as_string = std::tuple{some_three, none} + [](auto
//        const& first, auto const& second) {
//            return std::to_string(first * second);
//        };
//
//        static_assert(is_same_after_decaying<std::io<int>, decltype(sum)>);
//        static_assert(is_same_after_decaying<std::io<std::string>,
//        decltype(product_as_string)>);
//
//        EXPECT_TRUE(!sum.has_value());
//        EXPECT_TRUE(!product_as_string.has_value());
//    }
//
//    TEST(io, combine_should_returnCombined_when_notEmpty) {
//        auto const some_two = std::io<int>{2};
//        auto const some_three = std::io<int>{3};
//
//        auto const sum = some_two + some_three;
//        auto const product_as_string = std::tuple{some_three, some_two} +
//        [](auto const& first, auto const& second) {
//            return std::to_string(first * second);
//        };
//
//        static_assert(is_same_after_decaying<std::io<int>, decltype(sum)>);
//        static_assert(is_same_after_decaying<std::io<std::string>,
//        decltype(product_as_string)>);
//
//        EXPECT_TRUE(sum.has_value());
//        EXPECT_EQ(5, sum.value());
//        EXPECT_TRUE(product_as_string.has_value());
//        EXPECT_EQ("6", product_as_string.value());
//    }
//
//    TEST(io, wrap_should_returnANonEmptyMonad) {
//        auto const some_one = wrap<std::io>(1);
//
//        static_assert(is_same_after_decaying<std::io<int>,
//        decltype(some_one)>);
//
//        EXPECT_TRUE(some_one.has_value());
//        EXPECT_EQ(1, some_one.value());
//    }
//
//    TEST(io, bind_should_returnEmpty_when_empty) {
//        auto const none = std::io<int>{};
//        auto const mapped_none = none >>= [](auto v){ return
//        std::io{std::to_string(v * 10)}; };
//
//        static_assert(is_same_after_decaying<std::io<std::string>,
//        decltype(mapped_none)>);
//
//        EXPECT_TRUE(!mapped_none.has_value());
//    }
//
//    TEST(io, bind_should_returnMapped_when_notEmpty) {
//        auto const some_one = std::io<int>{1};
//        auto const mapped_some = some_one >>= [](auto v){ return
//        std::io{std::to_string(v * 10)}; };
//
//        static_assert(is_same_after_decaying<std::io<std::string>,
//        decltype(mapped_some)>);
//
//        EXPECT_TRUE(mapped_some.has_value());
//        EXPECT_EQ("10", mapped_some.value());
//    }

} // namespace