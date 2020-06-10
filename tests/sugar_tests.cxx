#include "catch.hpp"
#include "args/sugar.hpp"

using namespace args;
using namespace args::literals;

TEST_CASE("", "")
{
    auto o1 = 'a'_o | of_type<int>{};

    REQUIRE(std::is_same_v<decltype(o1), opt<int>>);
}
