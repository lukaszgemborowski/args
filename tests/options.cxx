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

TEST_CASE("Prase int value", "[args][opt]")
{
    auto o = args::opt{'a', 12};

    REQUIRE(o.found() == false);

    o.set_found();
    o.set_value("42");
    o.parse_complete();

    REQUIRE(o.found());
    REQUIRE(o.value() == 42);
}

TEST_CASE("Return default value", "[args][opt]")
{
    auto o = args::opt{'a', 12};

    o.parse_complete();

    REQUIRE(o.found() == false);
    REQUIRE(o.value() == 12);
}

TEST_CASE("Parse int value to external variable", "[args][opt]")
{
    int result = 0;
    auto o = args::opt{'a', args::save{result}};

    o.set_found();
    o.set_value("42");
    o.parse_complete();

    REQUIRE(o.found() == true);
    REQUIRE(result == 42);
}

TEST_CASE("Assign default value to external variable", "[args][opt]")
{
    int result = 0;
    auto o = args::opt{'a', args::save{result}, 12};

    o.parse_complete();

    REQUIRE(o.found() == false);
    REQUIRE(result == 12);
}

template<class T>
struct times_two {
    T operator()(T in) const { return 2 * in; }
};

TEST_CASE("Parse int and tranlate it with the same type", "[args][opt]")
{
    auto o = args::opt<int, times_two<int>>{'a'};

    o.set_found();
    o.set_value("21");
    o.parse_complete();

    REQUIRE(o.found());
    REQUIRE(o.value() == 42);
}

enum class TranslationResult {
    Zero, One, Other
};

struct translete_to_enum {
    template<class T>
    TranslationResult operator()(T in) {
        switch (in) {
        case 0: return TranslationResult::Zero;
        case 1: return TranslationResult::One;
        default: return TranslationResult::Other;
        }
    }
};

TEST_CASE("Parse int and translate it to different type", "[args][opt]")
{
    auto o = args::opt<int, translete_to_enum>{'a'};
    o.set_found();

    SECTION("parse zero")
    {
        o.set_value("0");
        o.parse_complete();
        REQUIRE(o.value() == TranslationResult::Zero);
    }

    SECTION("parse one")
    {
        o.set_value("1");
        o.parse_complete();
        REQUIRE(o.value() == TranslationResult::One);
    }

    SECTION("parse fourty two")
    {
        o.set_value("42");
        o.parse_complete();
        REQUIRE(o.value() == TranslationResult::Other);
    }
}

