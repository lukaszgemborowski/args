#include "catch.hpp"
#include "args/detail/short_parser.hpp"
#include "args/detail/opt_state.hpp"
#include "args/opt.hpp"
#include "helpers.hpp"

using namespace args;

TEST_CASE("Short options, separate", "[args][short_parser]")
{
    int a = 0;
    auto opts  = std::make_tuple(
        detail::opt_state{opt{'a', "", "", a}}
    );

    auto sp = detail::short_parser{opts, "-a", "10"};

    auto r = sp.parse();
    REQUIRE(r.success);
    REQUIRE(r.consumed);
    REQUIRE(a == 10);
}

TEST_CASE("Parse two options in one argument", "[args][short_parser]")
{
    int a = 0, b = 0;
    auto opts  = std::make_tuple(
        detail::opt_state{opt{'a', "", "", a}},
        detail::opt_state{opt{'b', "", "", b}}
    );

    auto sp = detail::short_parser{opts, "-ab", "20"};

    auto r = sp.parse();
    REQUIRE(r.success);
    REQUIRE(r.consumed);
    REQUIRE(a == 20);
    REQUIRE(b == 20);
}

TEST_CASE("Parse non consuming option", "[args][short_parser]")
{
    auto a = flag{false};
    auto opts = std::make_tuple(
        detail::opt_state{opt{'a', "", "", a}});

    auto sp = detail::short_parser{opts, "-a", ""};
    auto r = sp.parse();

    REQUIRE(r.consumed == false);
    REQUIRE(r.success == true);
    REQUIRE(a == true);
}

TEST_CASE("Parse consuming in one argument with value", "[args][short_parser][fail]")
{
    int a = 0;
    auto opts = std::make_tuple(
        detail::opt_state{opt{'a', "", "", a}}
    );

    auto sp = detail::short_parser{opts, "-a42", ""};
    auto r = sp.parse();

    REQUIRE(r.consumed == false);
    REQUIRE(r.success == true);
    REQUIRE(a == 42);
}

