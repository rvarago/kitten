#include <catch2/catch_all.hpp>

#include <optional>
#include <string>

#include <kitten/instances/variant.h>

#include "utils.h"

namespace {

using namespace rvarago::kitten;
using test::utils::is_same_after_decaying;

struct error_t final {
    explicit error_t(int _code) : code{_code} {
    }

    int const code;
};

SCENARIO("variant admits a multifunctor instance", "[variant]") {

    GIVEN("A variant") {

        AND_GIVEN("a function that maps all the choices") {

            auto choices_mapper =
                syntax::overloaded{[](int v) { return std::optional{v * 10}; }, [](std::string v) { return v; },
                                   [](error_t) { return error_t{-2}; }};

            WHEN("in an error choice") {

                auto const error_choice = std::variant<int, std::string, error_t>{error_t{-1}};

                THEN("return a mapped variant also in an error choice") {

                    auto const mapped_error = error_choice || choices_mapper;

                    static_assert(is_same_after_decaying<decltype(mapped_error),
                                                         std::variant<std::optional<int>, std::string, error_t>>);

                    CHECK(std::holds_alternative<error_t>(mapped_error));
                    CHECK(std::get<error_t>(mapped_error).code == -2);
                }
            }

            WHEN("in an integer choice") {

                auto const integer_choice = std::variant<int, std::string, error_t>{1};

                THEN("return a mapped variant also in an integer choice") {

                    auto const mapped_optional_of_integer = integer_choice || choices_mapper;

                    static_assert(is_same_after_decaying<decltype(mapped_optional_of_integer),
                                                         std::variant<std::optional<int>, std::string, error_t>>);

                    CHECK(std::holds_alternative<std::optional<int>>(mapped_optional_of_integer));
                    CHECK(std::get<std::optional<int>>(mapped_optional_of_integer) == std::optional{10});
                }
            }
        }
    }
}

}