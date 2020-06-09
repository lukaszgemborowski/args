#include "catch.hpp"
#include "args/detail/opt_state.hpp"
#include "args/opt.hpp"

using namespace args;

TEST_CASE("Create opt_state with a reference", "[args][opt_state]")
{
    opt<int> opt1{'x', "xxx", ""};
    auto os = detail::opt_state{opt1};

    opt1.value() = 42;
    REQUIRE(os.opt_.value() == 42);
}

TEST_CASE("Create opt_state with a temporary", "[args][opt_state]")
{
    int ext = 6;
    auto os = detail::opt_state{opt{'x', "", "", ext}};
    REQUIRE(os.opt_.value() == 6);
    os.opt_.value() = 42;
    REQUIRE(ext == 42);
}
