#include "catch.hpp"
#include "args/parser.hpp"
#include "helpers.hpp"

using namespace args2;

TEST_CASE("", "")
{
    auto o = opt<int>{'b', "", ""};
    int a;
    auto p = parser{
        opt{'a', "", "", a}, o};
    auto args = CreateArgs{
        {"-a", "10", "-b", "20", "unknown"}
    };

    p.parse(args.argc(), args.argv());

    REQUIRE(a == 10);
    REQUIRE(o.value() == 20);
}
