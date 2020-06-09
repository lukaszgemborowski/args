#include "catch.hpp"
#include "args/parser.hpp"
#include "helpers.hpp"

using namespace args2;

TEST_CASE("Construct with internal storage", "[args2][opt]")
{
    opt<int> opt1{'x', "xxx", ""};

    opt1.value() = 42;
    REQUIRE(opt1.value() == 42);
}

TEST_CASE("Construct with external storage", "[args2][opt]")
{
    int external = 6;
    opt o{'x', "xxx", "", external};

    REQUIRE(o.value() == 6);
    o.value() = 42;
    REQUIRE(o.value() == 42);
    REQUIRE(external == 42);
}

TEST_CASE("Create opt_state with a reference", "[args2][opt_state]")
{
    opt<int> opt1{'x', "xxx", ""};
    auto os = detail::opt_state{opt1};

    opt1.value() = 42;
    REQUIRE(os.opt_.value() == 42);
}

TEST_CASE("Create opt_state with a temporary", "[args2][opt_state]")
{
    int ext = 6;
    auto os = detail::opt_state{opt{'x', "", "", ext}};
    REQUIRE(os.opt_.value() == 6);
    os.opt_.value() = 42;
    REQUIRE(ext == 42);
}

TEST_CASE("Short options, separate", "[args2][short_parser]")
{
    int a = 0, b = 0;
    auto opts  = std::make_tuple(
        detail::opt_state{opt{'a', "", "", a}},
        detail::opt_state{opt{'b', "", "", b}}
    );

    auto args = CreateArgs{
        {"-a", "10", "-b", "20", "unknown"}
    };

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

