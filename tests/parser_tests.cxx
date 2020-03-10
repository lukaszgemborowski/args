#include "catch.hpp"
#include <args/args.hpp>

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

