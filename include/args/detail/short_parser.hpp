#ifndef ARGS_DETAIL_SHORT_PARSER_HPP
#define ARGS_DETAIL_SHORT_PARSER_HPP

#include "args/common.hpp"
#include "args/parser_result.hpp"
#include <type_traits>
#include <tuple>

ARGS_NAMESPACE_BEGIN

namespace detail {

template<class T>
struct short_parser
{
    short_parser(T& opts, int c, int argc, char **argv)
        : opts_ {opts}
        , c_ {c}
        , argc_ {argc}
        , argv_ {argv}
    {}

    using Idx = std::make_index_sequence<std::tuple_size_v<T>>;
    template<std::size_t... I>
    using Seq = std::index_sequence<I...>;

    auto parse()
    {
        return parse(Idx{});
    }

    template<std::size_t... I>
    auto parse(Seq<I...>)
    {
        parser_result result;
        (result.update(parse_one(std::get<I>(opts_))), ...);
        return result;
    }

    template<class P>
    parser_result parse_one(P &parser)
    {
        parser_result result;

        for (auto chr : std::string_view{argv_[c_]}) {
            if (parser.opt_.short_name() == chr) {
                result.update(parser.parse(c_, argc_, argv_));
            }
        }

        return result;
    }

    T&      opts_;
    int     c_;
    int     argc_;
    char**  argv_;
};

} // detail

ARGS_NAMESPACE_END

#endif
