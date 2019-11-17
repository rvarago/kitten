#include <gtest/gtest.h>

#include <iterator>
#include <string>
#include <kitten/instances/sequence_container.h>

namespace {

    using namespace rvarago::kitten;

    template <typename SequenceContainer, typename Index>
    decltype(auto) value_at(SequenceContainer const &container, Index index) {
        return *std::next(std::cbegin(container), index);
    }

    template <typename T>
    struct SequenceContainerTest : public ::testing::Test {
        using type = T;
    };

    // TODO: Generalize the tests to support template template parameter
    using sequence_containers = ::testing::Types<std::deque<int>, std::list<int>, std::vector<int>>;

    TYPED_TEST_CASE(SequenceContainerTest, sequence_containers);

    TYPED_TEST(SequenceContainerTest, map_should_returnEmpty_when_empty) {
        using T  = typename TestFixture::type;

        auto const empty = T{};
        auto const mapped_empty = empty | [](auto v){ return std::to_string(v * 10); };

        EXPECT_TRUE(mapped_empty.empty());
    }


    TYPED_TEST(SequenceContainerTest, map_should_returnMapped_when_notEmpty) {
        using T  = typename TestFixture::type;

        auto const container = T{1, 2};
        auto const mapped_container = container | [](auto v){ return std::to_string(v * 10); };

        EXPECT_TRUE(!mapped_container.empty());
        EXPECT_EQ(2, mapped_container.size());

        EXPECT_EQ("10", value_at(mapped_container, 0));
        EXPECT_EQ("20", value_at(mapped_container, 1));

    }

    TEST(deque, pure_should_returnANonEmptyApplicative) {
        // FIXME: Test for all the other containers
        auto const singleton = pure<std::deque>(1);

        EXPECT_TRUE(!singleton.empty());
        EXPECT_EQ(1, value_at(singleton, 0));
    }

    TEST(vector, pure_should_returnANonEmptyApplicative) {
        auto const singleton = pure<std::vector>(1);

        EXPECT_TRUE(!singleton.empty());
        EXPECT_EQ(1, value_at(singleton, 0));
    }

    TEST(list, pure_should_returnANonEmptyApplicative) {
        auto const singleton = pure<std::list>(1);

        EXPECT_TRUE(!singleton.empty());
        EXPECT_EQ(1, value_at(singleton, 0));
    }


    TYPED_TEST(SequenceContainerTest, combine_should_returnEmpty_when_notEmpty) {
        using T  = typename TestFixture::type;
        auto const empty = T{};
        auto const second_container = T{20, 30};

        auto const sum = empty + second_container;
        auto const product_as_string = std::tuple{second_container, empty} + [](auto const& first, auto const& second) {
            return std::to_string(first * second);
        };

        EXPECT_TRUE(sum.empty());

        EXPECT_TRUE(product_as_string.empty());
    }

    TYPED_TEST(SequenceContainerTest, combine_should_returnCombined_when_notEmpty) {
        using T  = typename TestFixture::type;
        auto const first_container = T{2, 3};
        auto const second_container = T{20, 30};

        auto const sum = first_container + second_container;
        auto const product_as_string = std::tuple{second_container, first_container} + [](auto const& first, auto const& second) {
            return std::to_string(first * second);
        };

        EXPECT_TRUE(!sum.empty());
        EXPECT_EQ(4, sum.size());

        EXPECT_EQ(22, value_at(sum, 0));
        EXPECT_EQ(32, value_at(sum, 1));
        EXPECT_EQ(23, value_at(sum, 2));
        EXPECT_EQ(33, value_at(sum, 3));

        EXPECT_TRUE(!product_as_string.empty());
        EXPECT_EQ(4, product_as_string.size());

        EXPECT_EQ("40", value_at(product_as_string, 0));
        EXPECT_EQ("60", value_at(product_as_string, 1));
        EXPECT_EQ("60", value_at(product_as_string, 2));
        EXPECT_EQ("90", value_at(product_as_string, 3));
    }

    TEST(deque, wrap_should_returnANonEmptyMonad) {
        // FIXME: Test for all the other containers
        auto const singleton = wrap<std::deque>(1);

        EXPECT_TRUE(!singleton.empty());
        EXPECT_EQ(1, value_at(singleton, 0));
    }

    TEST(vector, wrap_should_returnANonEmptyMonad) {
        auto const singleton = wrap<std::vector>(1);

        EXPECT_TRUE(!singleton.empty());
        EXPECT_EQ(1, value_at(singleton, 0));
    }

    TEST(list, wrap_should_returnANonEmptyMonad) {
        auto const singleton = wrap<std::list>(1);

        EXPECT_TRUE(!singleton.empty());
        EXPECT_EQ(1, value_at(singleton, 0));
    }

    TYPED_TEST(SequenceContainerTest, bind_should_returnEmpty_when_empty) {
        using T  = typename TestFixture::type;

        auto const empty = T{};
        auto const mapped_empty = empty >> [](auto v){ return std::vector{std::to_string(v * 10), std::to_string(v * 100)}; };

        EXPECT_TRUE(mapped_empty.empty());
    }

    TYPED_TEST(SequenceContainerTest, bind_should_returnMapped_when_notEmpty) {
        using T  = typename TestFixture::type;

        auto const container = T{1, 2};
        auto const mapped_container = container >> [](auto v){ return std::vector{std::to_string(v * 10), std::to_string(v * 100)}; };

        EXPECT_TRUE(!mapped_container.empty());
        EXPECT_EQ(4, mapped_container.size());

        EXPECT_EQ("10", value_at(mapped_container, 0));
        EXPECT_EQ("100", value_at(mapped_container, 1));
        EXPECT_EQ("20", value_at(mapped_container, 2));
        EXPECT_EQ("200", value_at(mapped_container, 3));
    }

}