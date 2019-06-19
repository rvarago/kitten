#include <gtest/gtest.h>

#include <optional>
#include <string>
#include <type_traits>

#include <kitten/instances/variant.h>

using namespace rvarago::kitten;

namespace {

    template <typename T1, typename T2>
    inline constexpr bool is_same_after_decaying = std::is_same<std::decay_t<T1>, std::decay_t<T2>>::value;

    struct error_t final {
        explicit error_t(int _code) : code{_code} {}
        int const code;
    };

    TEST(variant, nfmap_should_returnEmpty_when_empty) {
        auto const error = std::variant<int, std::string, error_t>{error_t{-1}};
        auto const mapped_error = error || syntax::overloaded {
                [](int v) { return std::optional{v * 10}; },
                [](std::string v) { return v; },
                [](error_t) { return error_t{-2}; },
        };

        static_assert(is_same_after_decaying<std::variant<std::optional<int>, std::string, error_t>, decltype(mapped_error)>);

        EXPECT_TRUE(std::holds_alternative<error_t>(mapped_error));
        EXPECT_EQ(-2, std::get<error_t>(mapped_error).code);
    }

    TEST(variant, nfmap_should_returnMapped_when_notEmpty) {
        auto const integer = std::variant<int, std::string, error_t>{1};
        auto const mapped_optional_of_integer = integer|| syntax::overloaded {
                [](int v) { return std::optional{v * 10}; },
                [](auto v) { return v; }
        };

        static_assert(is_same_after_decaying<std::variant<std::optional<int>, std::string, error_t>, decltype(mapped_optional_of_integer)>);

        EXPECT_TRUE(std::holds_alternative<std::optional<int>>(mapped_optional_of_integer));
        EXPECT_EQ(std::optional{10}, std::get<std::optional<int>>(mapped_optional_of_integer));
    }

}