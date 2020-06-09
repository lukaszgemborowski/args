#ifndef ARGS_OPT_PARSER_HPP
#define ARGS_OPT_PARSER_HPP

#include "args/common.hpp"
#include "args/parser_result.hpp"
#include <sstream>

ARGS_NAMESPACE_BEGIN

template<class T>
struct opt_parser;

struct flag {
    explicit flag(bool value) : value_ {value} {}
    flag() = default;
    operator bool() const { return value_; }
    flag& operator=(bool value) {
        value_ = value;
        return *this;
    }

private:
    bool value_ = false;
};

template<>
struct opt_parser<flag>
{
    struct data {};
    using storage = bool;

    template<class S>
    static parser_result parse(
        S& state,
        std::string_view,
        std::string_view)
    {
        state.opt_.value() = true;
        return {true, false};
    }
};

template<class T>
struct opt_parser
{
    struct data {};
    using storage = T;

    template<class S>
    static parser_result parse(
        S &state,
        std::string_view current,
        std::string_view next)
    {
        if (next.length() == 0)
            return {false, false};

        std::stringstream ss;
        ss << next;
        ss >> state.opt_.value();
        return {true, true};
    }
};

ARGS_NAMESPACE_END

#endif
