#include <catch2/catch.hpp>

#include <kitten/instances/optional.h>
#include <string>

#include "utils.h"
#include <functional>

namespace {

using namespace std::string_literals;

using namespace rvarago::kitten;
using test::utils::is_same_after_decaying;

SCENARIO("optional admits functor, applicative, and monad instances", "[optional]") {

    GIVEN("An optional") {

        AND_GIVEN("a functor instance") {

            AND_GIVEN("fmap") {

                auto to_string = [](auto const v) { return std::to_string(v); };

                WHEN("empty") {

                    std::optional<int> const none;

                    THEN("return an empty optional") {

                        auto const none_of_string = none | to_string;

                        static_assert(is_same_after_decaying<decltype(none_of_string), std::optional<std::string>>);

                        CHECK(!none_of_string.has_value());
                    }
                }

                WHEN("not empty") {

                    auto const some_one = std::optional{1};

                    THEN("return a non-empty optional containing the mapped value") {

                        auto const some_one_of_string = some_one | to_string;

                        static_assert(is_same_after_decaying<decltype(some_one_of_string), std::optional<std::string>>);

                        CHECK(some_one_of_string.has_value());
                        CHECK(some_one_of_string.value() == "1"s);
                    }
                }
            }

            AND_GIVEN("liftF") {

                auto to_string_lifted = liftF<std::optional>([](auto const v) { return std::to_string(v); });

                WHEN("empty") {

                    std::optional<int> const none;

                    THEN("return an empty optional") {

                        auto const none_of_string = to_string_lifted(none);

                        static_assert(is_same_after_decaying<decltype(none_of_string), std::optional<std::string>>);

                        CHECK(!none_of_string.has_value());
                    }
                }

                WHEN("not empty") {

                    auto const some_one = std::optional{1};

                    THEN("return an empty optional") {

                        auto const some_one_of_string = to_string_lifted(some_one);

                        static_assert(is_same_after_decaying<decltype(some_one_of_string), std::optional<std::string>>);

                        CHECK(some_one_of_string.has_value());
                        CHECK(some_one_of_string.value() == "1"s);
                    }
                }
            }
        }

        AND_GIVEN("an applicative instance") {

            AND_GIVEN("pure") {

                THEN("lift into a non-empty optional") {

                    auto const some_one = pure<std::optional>("1"s);

                    static_assert(is_same_after_decaying<decltype(some_one), std::optional<std::string>>);

                    CHECK(some_one.has_value());
                    CHECK(some_one.value() == "1"s);
                }
            }

            AND_GIVEN("combine") {

                auto to_product_as_string = [](auto const &a, auto const &b) { return std::to_string(a * b); };

                WHEN("empty") {

                    std::optional<int> const none;

                    THEN("return an empty optional") {

                        auto const some_three = std::optional{3};
                        auto const none_of_string = std::tuple{none, some_three} + to_product_as_string;

                        static_assert(is_same_after_decaying<decltype(none_of_string), std::optional<std::string>>);

                        CHECK(!none_of_string.has_value());
                    }
                }

                WHEN("not empty") {

                    auto const some_two = std::optional{2};

                    THEN("return a non-empty optional with the combined value") {

                        auto const some_three = std::optional<int>{3};
                        auto const product_of_string = std::tuple{some_two, some_three} + to_product_as_string;

                        static_assert(is_same_after_decaying<decltype(product_of_string), std::optional<std::string>>);

                        CHECK(product_of_string.has_value());
                        CHECK(product_of_string.value() == "6"s);
                    }
                }
            }

            AND_GIVEN("liftA2") {

                auto const lifted_plus = liftA2<std::optional>(std::plus<int>{});

                WHEN("any is empty") {

                    THEN("the lifted function that adds to integers into one should return an empty optional") {

                        std::optional<int> const none = std::nullopt;
                        auto const some = std::optional{42};

                        auto const left_sum = lifted_plus(none, some);
                        auto const right_sum = lifted_plus(some, none);

                        static_assert(is_same_after_decaying<decltype(left_sum), std::optional<int>>);
                        static_assert(is_same_after_decaying<decltype(right_sum), std::optional<int>>);

                        CHECK(!left_sum.has_value());
                        CHECK(!right_sum.has_value());
                    }
                }

                WHEN("both are not empty") {

                    THEN("the lifted function that adds to integers into one should return a filled optional "
                         "containing the sum") {

                        auto const some_1 = std::optional{1};
                        auto const some_2 = std::optional{2};

                        auto const sum = lifted_plus(some_1, some_2);

                        static_assert(is_same_after_decaying<decltype(sum), std::optional<int>>);

                        CHECK(sum.has_value());
                        CHECK(sum.value() == 3);
                    }
                }
            }
        }

        AND_GIVEN("a monad instance") {

            AND_GIVEN("wrap") {

                THEN("lift into a non-empty optional") {

                    auto const some_one = wrap<std::optional>("1"s);

                    static_assert(is_same_after_decaying<decltype(some_one), std::optional<std::string>>);

                    CHECK(some_one.has_value());
                    CHECK(some_one.value() == "1"s);
                }
            }

            AND_GIVEN("bind") {

                auto to_optional_string = [](auto v) { return std::optional{std::to_string(v)}; };
                auto to_optional_int = [](auto v) { return std::optional{std::stoi(v)}; };

                WHEN("empty") {

                    std::optional<int> const none;

                    THEN("return an empty optional") {

                        auto const none_of_string = none >> to_optional_string >> to_optional_int;

                        static_assert(is_same_after_decaying<decltype(none_of_string), std::optional<int>>);

                        CHECK(!none_of_string.has_value());
                    }
                }

                WHEN("not empty") {

                    auto const some_one = std::optional{1};

                    THEN("return a non-empty optional containing the bound value") {

                        auto const some_one_of_string = some_one >> to_optional_string >> to_optional_int;

                        static_assert(is_same_after_decaying<decltype(some_one_of_string), std::optional<int>>);

                        CHECK(some_one_of_string.has_value());
                        CHECK(some_one_of_string.value() == 1);
                    }
                }
            }
        }
    }
}
}
