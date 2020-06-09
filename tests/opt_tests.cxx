#include "catch.hpp"
#include "args/opt.hpp"

using namespace args;

TEST_CASE("Construct with internal storage", "[args][opt]")
{
    opt<int> opt1{'x', "xxx", ""};

    opt1.value() = 42;
    REQUIRE(opt1.value() == 42);
}

TEST_CASE("Construct with external storage", "[args][opt]")
{
    int external = 6;
    opt o{'x', "xxx", "", external};

    REQUIRE(o.value() == 6);
    o.value() = 42;
    REQUIRE(o.value() == 42);
    REQUIRE(external == 42);
}

