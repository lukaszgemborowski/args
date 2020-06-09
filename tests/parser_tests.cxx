#include "catch.hpp"
#include <args/args.hpp>
#include "helpers.hpp"

TEST_CASE("Parse int, value found", "[args][parser]")
{
    auto opt_a = args::opt{'a', 12};
    auto cmd = CreateArgs{"-a", "42"};
    auto parser = args::parser{opt_a};
    parser.parse(cmd.argc(), cmd.argv());

    REQUIRE(opt_a.found());
    REQUIRE(opt_a.value() == 42);
}


TEST_CASE("Parse int, not found", "[args][parser]")
{
    auto opt_a = args::opt{'a', 12};
    auto cmd = CreateArgs{};
    auto parser = args::parser{opt_a};
    parser.parse(cmd.argc(), cmd.argv());

    REQUIRE(opt_a.found() == false);
    REQUIRE(opt_a.value() == 12);
}

TEST_CASE("Parse int, save to variable, value found", "[args][parser]")
{
    int val_a = 0;
    auto opt_a = args::opt{'a', 12, args::save{val_a}};
    auto cmd = CreateArgs{"-a", "42"};
    auto parser = args::parser{opt_a};
    parser.parse(cmd.argc(), cmd.argv());

    REQUIRE(opt_a.found() == true);
    REQUIRE(opt_a.value() == 42);
    REQUIRE(val_a == 42);
}

