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

namespace {
struct OrOperatorCompilationTest {
    template<class Opt>
        // this overloaded will be chosen if | operator can be applied to an lvalue reference
    static auto test(Opt &o) -> decltype(o | description{""}, std::false_type{});
        // choose this instead
    static auto test(...) -> std::true_type;
};
}

TEST_CASE("Construct with description", "[args][opt]")
{
    auto o = opt<int>{'x'} | description{"info"};
    REQUIRE(o.description() == "info");

    // should not be possible to mutate the state of opt<>
    // | operator can be used only on a rvalue reference object
    REQUIRE(std::is_same_v<std::true_type, decltype(OrOperatorCompilationTest::test(o))>);
}

