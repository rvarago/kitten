#include <gtest/gtest.h>

#include <iterator>
#include <string>
#include <kitten/instances/sequence_container.h>

using namespace rvarago::kitten;

namespace {

    template <typename SequenceContainer, typename Index>
    decltype(auto) value_at(SequenceContainer const &container, Index index) {
        return *std::next(std::cbegin(container), index);
    }

    template <typename T>
    struct SequenceContainerTest : public ::testing::Test {
        using type = T;
    };

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