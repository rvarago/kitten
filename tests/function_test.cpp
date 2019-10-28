#include <gtest/gtest.h>

#include <string>
#include <kitten/instances/function.h>

namespace {

    using namespace rvarago::kitten;

    using namespace std::string_literals;
    using namespace rvarago::kitten;
    using namespace rvarago::kitten::types;

    char void_to_char() {
        return 'x';
    }

    int char_to_int(char) {
        return 42;
    }

    std::string int_to_string(int) {
        return "10"s;
    }

    TEST(function, map_should_returnTheFunctionsComposition) {
        auto const char_to_string = fn(char_to_int) | fn(int_to_string);
        auto const void_to_string = fn(void_to_char) | fn(char_to_int) | fn(int_to_string);

        EXPECT_EQ("10"s, char_to_string('x'));
        EXPECT_EQ("10"s, void_to_string());
    }

}