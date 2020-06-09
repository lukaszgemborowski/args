#ifndef ARGS_OPT_PARSER_HPP
#define ARGS_OPT_PARSER_HPP

#include "args/common.hpp"
#include "args/parser_result.hpp"
#include <sstream>

ARGS_NAMESPACE_BEGIN

template<class T>
struct opt_parser
{
    struct data {};

    template<class S>
    static parser_result parse(
        S &state,
        int c,
        int argc,
        char **argv)
    {
        if (c == (argc-1))
            return {false, false};

        std::stringstream ss;
        ss << argv[c + 1];
        ss >> state.opt_.value();
        return {true, true};
    }
};

ARGS_NAMESPACE_END

#endif
