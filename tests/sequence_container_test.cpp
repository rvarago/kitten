#include <catch2/catch.hpp>

#include <iterator>
#include <kitten/instances/sequence_container.h>
#include <string>

#include "utils.h"

namespace {

using namespace std::string_literals;

using namespace rvarago::kitten;
using test::utils::is_same_after_decaying;

template <typename Range, typename Index>
decltype(auto) value_at(Range const &container, Index index) {
    return *std::next(std::cbegin(container), index);
}

template <typename T, typename Allocator = std::allocator<T>>
using SequenceContainer = std::vector<T, Allocator>;

SCENARIO("SequenceContainer admits functor, applicative, and monad instances", "[SequenceContainer]") {

    GIVEN("A SequenceContainer") {

        AND_GIVEN("a functor instance") {

            AND_GIVEN("fmap") {

                auto to_string = [](auto const v) { return std::to_string(v); };

                WHEN("when empty") {

                    SequenceContainer<int> const empty;

                    THEN("return an empty SequenceContainer") {

                        auto const empty_of_strings = empty | to_string;

                        static_assert(
                            is_same_after_decaying<decltype(empty_of_strings), SequenceContainer<std::string>>);

                        CHECK(empty_of_strings.empty());
                    }
                }

                WHEN("when not empty") {

                    auto const container_of_ints = SequenceContainer<int>{1, 2};

                    THEN("return a non-empty SequenceContainer containing the mapped values") {

                        auto const container_of_strings = container_of_ints | to_string;

                        static_assert(
                            is_same_after_decaying<decltype(container_of_strings), SequenceContainer<std::string>>);

                        CHECK(container_of_strings.size() == 2);
                        CHECK(value_at(container_of_strings, 0) == "1"s);
                        CHECK(value_at(container_of_strings, 1) == "2"s);
                    }
                }
            }
        }

        AND_GIVEN("an applicative instance") {

            AND_GIVEN("pure") {

                THEN("lift into a non-empty SequenceContainer") {

                    auto const singleton = pure<SequenceContainer>("1"s);

                    static_assert(is_same_after_decaying<decltype(singleton), SequenceContainer<std::string>>);

                    CHECK(singleton.size() == 1);
                    CHECK(value_at(singleton, 0) == "1"s);
                }
            }

            AND_GIVEN("combine") {

                auto to_product_as_string = [](auto const &a, auto const &b) {
                    return std::to_string(std::stoi(a) * b);
                };

                WHEN("when empty") {

                    SequenceContainer<std::string> const empty_of_string;

                    THEN("return an empty SequenceContainer") {

                        auto const container_int = SequenceContainer<int>{20, 30};
                        auto const product_of_string =
                            std::tuple{empty_of_string, container_int} + to_product_as_string;

                        static_assert(
                            is_same_after_decaying<decltype(product_of_string), SequenceContainer<std::string>>);

                        CHECK(product_of_string.empty());
                    }
                }

                WHEN("when not empty") {

                    auto const first_container_of_string = SequenceContainer<std::string>{"2", "3"};

                    THEN("return a non-empty SequenceContainer with the combined value") {

                        auto const second_container_of_int = SequenceContainer<int>{20, 30};
                        auto const product_of_string =
                            std::tuple{first_container_of_string, second_container_of_int} + to_product_as_string;

                        static_assert(
                            is_same_after_decaying<decltype(product_of_string), SequenceContainer<std::string>>);

                        CHECK(product_of_string.size() == 4);
                        CHECK(value_at(product_of_string, 0) == "40"s);
                        CHECK(value_at(product_of_string, 1) == "60"s);
                        CHECK(value_at(product_of_string, 2) == "60"s);
                        CHECK(value_at(product_of_string, 3) == "90"s);
                    }
                }
            }
        }

        AND_GIVEN("a monad instance") {

            AND_GIVEN("wrap") {

                THEN("lift into a non-empty SequenceContainer") {

                    auto const singleton = wrap<SequenceContainer>("1"s);

                    static_assert(is_same_after_decaying<decltype(singleton), SequenceContainer<std::string>>);

                    CHECK(singleton.size() == 1);
                    CHECK(value_at(singleton, 0) == "1"s);
                }
            }

            AND_GIVEN("bind") {

                auto to_SequenceContainer_string = [](auto v) {
                    return SequenceContainer<std::string>{std::to_string(v), std::to_string(v)};
                };

                WHEN("when empty") {

                    SequenceContainer<int> const empty;

                    THEN("return an empty SequenceContainer") {

                        auto const empty_of_string = SequenceContainer<int>{} >>= to_SequenceContainer_string;

                        static_assert(
                            is_same_after_decaying<decltype(empty_of_string), SequenceContainer<std::string>>);

                        CHECK(empty_of_string.empty());
                    }
                }

                WHEN("when not empty") {

                    auto const container = SequenceContainer<int>{1, 2};

                    THEN("return a non-empty SequenceContainer containing the bound value") {

                        auto const container_of_string = container >>= to_SequenceContainer_string;

                        static_assert(
                            is_same_after_decaying<decltype(container_of_string), SequenceContainer<std::string>>);

                        CHECK(container_of_string.size() == 4);
                        CHECK(value_at(container_of_string, 0) == "1"s);
                        CHECK(value_at(container_of_string, 1) == "1"s);
                        CHECK(value_at(container_of_string, 2) == "2"s);
                        CHECK(value_at(container_of_string, 3) == "2"s);
                    }
                }
            }
        }
    }
}
}
