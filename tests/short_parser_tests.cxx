#include "catch.hpp"
#include "args/detail/short_parser.hpp"
#include "args/detail/opt_state.hpp"
#include "args/opt.hpp"
#include "helpers.hpp"

using namespace args;

TEST_CASE("Short options, separate", "[args][short_parser]")
{
    int a = 0, b = 0;
    auto opts  = std::make_tuple(
        detail::opt_state{opt{'a', "", "", a}},
        detail::opt_state{opt{'b', "", "", b}}
    );

    auto args = CreateArgs{"-a", "10", "-b", "20", "unknown"};

    SECTION("Parse first argument")
    {
        auto sp = detail::short_parser{opts, 1, args.argc(), args.argv()};

        auto r = sp.parse();
        REQUIRE(r.success);
        REQUIRE(r.consumed);
        REQUIRE(a == 10);
        REQUIRE(b == 0);
    }

    SECTION("Parse second argument")
    {
        auto sp = detail::short_parser{opts, 3, args.argc(), args.argv()};
        sp.parse();
        REQUIRE(a == 0);
        REQUIRE(b == 20);
    }
}

TEST_CASE("Parse two options in one argument", "[args][short_parser]")
{
    int a = 0, b = 0;
    auto opts  = std::make_tuple(
        detail::opt_state{opt{'a', "", "", a}},
        detail::opt_state{opt{'b', "", "", b}}
    );

    auto args = CreateArgs{"-ab", "20"};

    auto sp = detail::short_parser{opts, 1, args.argc(), args.argv()};

    auto r = sp.parse();
    REQUIRE(r.success);
    REQUIRE(r.consumed);
    REQUIRE(a == 20);
    REQUIRE(b == 20);
}
