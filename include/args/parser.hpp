#ifndef ARGS_PARSER_HPP
#define ARGS_PARSER_HPP

#include "args/common.hpp"
#include "args/parser_result.hpp"
#include "args/opt_parser.hpp"
#include "args/opt.hpp"
#include "args/detail/short_parser.hpp"
#include "args/detail/positional_parser.hpp"
#include "args/detail/opt_state.hpp"
#include "args/detail/detail.hpp"
#include <tuple>

ARGS_NAMESPACE_BEGIN

template<class T>
struct parser {
    template<class... Opt>
    parser(Opt&&... opts)
        : opts_ {opts...}
    {
    }

    void parse(int argc, char **argv)
    {
        for (int c = 1; c < argc; ++c) {
            auto r = try_parse(c, argc, argv);

            if (r.consumed)
                ++c;
        }
    }

private:
    parser_result try_parse(int c, int argc, char **argv)
    {
        auto first = std::string_view{argv[c]};
        auto second = c <(argc - 1) ?
                      std::string_view{argv[c + 1]} :
                      std::string_view{};

        if (detail::is_short(argv[c])) {
            auto p = detail::short_parser{
                opts_,
                first,
                second};
            return p.parse();
        } else if (detail::is_long(argv[c])) {
            // TODO: implement
        } else {
            // this is positional argument
            auto p = detail::positional_parser{
                opts_,
                first,
                second
            };
            return p.parse();
        }

        return {false, false};
    }

private:
    T opts_;
};

template<class... Opts>
parser(Opts&&... opts) -> parser<std::tuple<detail::opt_state<Opts>...>>;

ARGS_NAMESPACE_END

#endif // ARGS_PARSER_HPP
