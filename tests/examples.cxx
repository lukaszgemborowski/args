#include "catch.hpp"
#include "args/parser.hpp"
#include "args/sugar.hpp"
#include "helpers.hpp"

TEST_CASE("Create few options", "[args][example]")
{
    using namespace args;

    // variable to save argument from command line
    auto some_value = 0;

    // stand-alone option holding a value
    auto option = opt<std::string>{'p', "", "path to something"};

    // create a parser
    auto p = parser{
        opt{'x', "", "some value", some_value},
        option
    };

    // just a helper, like calling app with: ./app -p /path/to/smth -x42
    auto ar = CreateArgs{"-p", "/path/to/smth", "-x42"};

    // parse it
    p.parse(ar.argc(), ar.argv());

    REQUIRE(some_value == 42);
    REQUIRE(option.value() == "/path/to/smth");
}
