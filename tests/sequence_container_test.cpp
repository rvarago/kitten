#include <catch2/catch.hpp>

#include "utils.h"
#include <functional>
#include <iterator>
#include <kitten/instances/sequence_container.h>
#include <string>

namespace {

using namespace std::string_literals;

using namespace rvarago::kitten;
using test::utils::is_same_after_decaying;

template <typename T, typename Allocator = std::allocator<T>>
using SequenceContainer = std::vector<T, Allocator>;

SCENARIO("SequenceContainer admits functor, applicative, and monad instances", "[SequenceContainer]") {

    GIVEN("A SequenceContainer") {

        AND_GIVEN("a functor instance") {

            AND_GIVEN("fmap") {

                auto to_string = [](auto const v) { return std::to_string(v); };

                WHEN("empty") {

                    SequenceContainer<int> const empty;

                    THEN("return an empty SequenceContainer") {

                        auto const empty_of_strings = empty | to_string;

                        static_assert(
                            is_same_after_decaying<decltype(empty_of_strings), SequenceContainer<std::string>>);

                        CHECK(empty_of_strings.empty());
                    }
                }

                WHEN("not empty") {

                    auto const container_of_ints = SequenceContainer<int>{1, 2};

                    THEN("return a non-empty SequenceContainer containing the mapped values") {

                        auto const container_of_strings = container_of_ints | to_string;

                        static_assert(
                            is_same_after_decaying<decltype(container_of_strings), SequenceContainer<std::string>>);

                        CHECK(container_of_strings.size() == 2);
                        CHECK(container_of_strings == SequenceContainer<std::string>{"1", "2"});
                    }
                }
            }

            AND_GIVEN("liftF") {

                auto to_string_lifted = liftF<SequenceContainer>([](auto const v) { return std::to_string(v); });

                WHEN("empty") {

                    SequenceContainer<int> const empty;

                    THEN("return an empty SequenceContainer") {

                        auto const empty_of_strings = to_string_lifted(empty);

                        static_assert(
                            is_same_after_decaying<decltype(empty_of_strings), SequenceContainer<std::string>>);

                        CHECK(empty_of_strings.empty());
                    }
                }

                WHEN("not empty") {

                    auto const container_of_ints = SequenceContainer<int>{1, 2};

                    THEN("return a non-empty SequenceContainer containing the mapped values") {

                        auto const container_of_strings = to_string_lifted(container_of_ints);

                        static_assert(
                            is_same_after_decaying<decltype(container_of_strings), SequenceContainer<std::string>>);

                        CHECK(container_of_strings.size() == 2);
                        CHECK(container_of_strings == SequenceContainer<std::string>{"1", "2"});
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
                    CHECK(singleton == SequenceContainer<std::string>{"1"});
                }
            }

            AND_GIVEN("combine") {

                auto to_product_as_string = [](auto const &a, auto const &b) {
                    return std::to_string(std::stoi(a) * b);
                };

                WHEN("empty") {

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

                WHEN("not empty") {

                    auto const first_container_of_string = SequenceContainer<std::string>{"2", "3"};

                    THEN("return a non-empty SequenceContainer with the combined value") {

                        auto const second_container_of_int = SequenceContainer<int>{20, 30};
                        auto const product_of_string =
                            std::tuple{first_container_of_string, second_container_of_int} + to_product_as_string;

                        static_assert(
                            is_same_after_decaying<decltype(product_of_string), SequenceContainer<std::string>>);

                        CHECK(product_of_string.size() == 4);
                        CHECK(product_of_string == SequenceContainer<std::string>{"40", "60", "60", "90"});
                    }
                }
            }

            AND_GIVEN("liftA2") {

                auto const lifted_plus = liftA2<SequenceContainer>(std::plus<int>{});

                WHEN("any is empty") {

                    THEN("the lifted function that adds to integers into one should return an empty optional") {

                        SequenceContainer<int> const empty{};
                        auto const singleton_one = SequenceContainer<int>{1};

                        auto const left_sum = lifted_plus(empty, singleton_one);
                        auto const right_sum = lifted_plus(singleton_one, empty);

                        static_assert(is_same_after_decaying<decltype(left_sum), SequenceContainer<int>>);
                        static_assert(is_same_after_decaying<decltype(right_sum), SequenceContainer<int>>);

                        CHECK(left_sum.empty());
                        CHECK(right_sum.empty());
                    }
                }

                WHEN("both are not empty") {

                    THEN("the lifted function that adds to integers into one should return a filled optional "
                         "containing the sum") {

                        auto const first_container = SequenceContainer<int>{1, 2};
                        auto const second_container = SequenceContainer<int>{10, 20};

                        auto const sum = lifted_plus(first_container, second_container);

                        static_assert(is_same_after_decaying<decltype(sum), SequenceContainer<int>>);

                        CHECK(sum.size() == 4);
                        CHECK(sum == SequenceContainer<int>{11, 21, 12, 22});
                    }
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
                CHECK(singleton == SequenceContainer<std::string>{"1"});
            }
        }

        AND_GIVEN("and_then") {

            auto to_SequenceContainer_string = [](auto v) {
                return SequenceContainer<std::string>{std::to_string(v), std::to_string(v)};
            };

            WHEN("empty") {

                SequenceContainer<int> const empty;

                THEN("return an empty SequenceContainer") {

                    auto const empty_of_string = SequenceContainer<int>{} >> to_SequenceContainer_string;

                    static_assert(is_same_after_decaying<decltype(empty_of_string), SequenceContainer<std::string>>);

                    CHECK(empty_of_string.empty());
                }
            }

            WHEN("not empty") {

                auto const container = SequenceContainer<int>{1, 2};

                THEN("return a non-empty SequenceContainer containing the bound value") {

                    auto const container_of_string = container >> to_SequenceContainer_string;

                    static_assert(
                        is_same_after_decaying<decltype(container_of_string), SequenceContainer<std::string>>);

                    CHECK(container_of_string.size() == 4);
                    CHECK(container_of_string == SequenceContainer<std::string>{"1", "1", "2", "2"});
                }
            }
        }
    }
}
}
