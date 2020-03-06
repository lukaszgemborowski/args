#include "catch.hpp"
#include <args/args.hpp>

using namespace args;
using namespace std::string_literals;

struct CreateArgs
{
    CreateArgs(std::initializer_list<std::string> a)
    {
        args_.push_back({});

        for (auto v : a) {
            std::vector<char> current{
                v.begin(), v.end()};
            current.push_back(0);
            args_.push_back(current);
        }

        for (auto &v : args_) {
            argv_.push_back(&v[0]);
        }
    }

    int argc() const
    {
        return args_.size();
    }

    char** argv()
    {
        return &argv_[0];
    }


private:
    std::vector<char *> argv_;
    std::vector<std::vector<char>> args_;
};

TEST_CASE("Parse several types of options", "[args]")
{
    auto argBool = opt{'b', false};
    auto argInt = opt{'i', 0};
    auto argString = opt{'s', ""s};
    int intValue = 0;

    auto holder = CreateArgs {
        "tests", "-b", "-i", "42", "-s", "foo", "-x", "24"
    };

    auto p = parser{argBool, argInt, argString, assign{'x', intValue}};
    p.parse(holder.argc(), holder.argv());

    CHECK(argBool == true);
    CHECK(argInt == 42);
    CHECK(*argString.value() == "foo"s);
    CHECK(intValue == 24);
}
