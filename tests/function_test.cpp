#include <catch2/catch.hpp>

#include <kitten/instances/function.h>
#include <string>

namespace {

using namespace std::string_literals;

using namespace rvarago::kitten;
using namespace types;

SCENARIO("function_wrapper admits a functor instance", "[function_wrapper]") {

    GIVEN("a function from double to int") {

        auto double_to_int = [](double v) -> int { return static_cast<int>(v); };

        AND_GIVEN("a function from int to string") {

            auto int_to_string = [](int v) -> std::string { return std::to_string(v); };

            WHEN("they are wrapped in function wrappers") {

                AND_WHEN("composed together") {

                    THEN("return the composition of a function wrapper from double to string") {

                        auto const double_to_string = fn(double_to_int) | fn(int_to_string);

                        CHECK(double_to_string(3.14) == "3"s);
                    }
                }
            }
        }
    }
}

}