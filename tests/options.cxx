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

TEST_CASE("Default value returned from opt", "[args][opt]")
{
    auto o = args::opt{'a', 12};

    REQUIRE(o.found() == false);
    REQUIRE(o.value() == 12);

    o.set_found();
    o.set_value("42");

    REQUIRE(o.found());
    REQUIRE(o.value() == 42);
}

