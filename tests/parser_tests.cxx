#include "catch.hpp"
#include "args/parser.hpp"
#include "helpers.hpp"

using namespace args;

TEST_CASE("Parse two short options in two arguments", "[args][parser]")
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

TEST_CASE("Parse one consuming and one non consuming", "[args][parser]")
{
    flag a;
    int b;

    auto p = parser{
        opt{'a', "", "", a},
        opt{'b', "", "", b}
    };

    SECTION("first")
    {
        auto args = CreateArgs{"-a", "-b", "42"};
        p.parse(args.argc(), args.argv());
    }

    SECTION("second")
    {
        auto args = CreateArgs{"-b", "42", "-a"};
        p.parse(args.argc(), args.argv());
    }

    SECTION("third")
    {
        auto args = CreateArgs{"-ab", "42"};
        p.parse(args.argc(), args.argv());
    }

    SECTION("fourth")
    {
        auto args = CreateArgs{"-ba", "42"};
        p.parse(args.argc(), args.argv());
    }

    REQUIRE(a == true);
    REQUIRE(b == 42);
}

