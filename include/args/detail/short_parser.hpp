#ifndef ARGS_DETAIL_SHORT_PARSER_HPP
#define ARGS_DETAIL_SHORT_PARSER_HPP

#include "args/common.hpp"
#include "args/parser_result.hpp"
#include <type_traits>
#include <string_view>
#include <tuple>

ARGS_NAMESPACE_BEGIN

namespace detail {

template<class T>
struct short_parser
{
    short_parser(T& opts, std::string_view current, std::string_view next)
        : opts_ {opts}
        , current_ {current}
        , next_ {next}
    {}

    using Idx = std::make_index_sequence<std::tuple_size_v<T>>;
    template<std::size_t... I>
    using Seq = std::index_sequence<I...>;

    auto parse()
    {
        return parse(Idx{});
    }

private:
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

        for (auto chr : current_) {
            if (parser.opt_.short_name() == chr) {
                result.update(parser.parse(current_, next_));
                continue;
            }
        }

        return result;
    }

    T&                  opts_;
    std::string_view    current_;
    std::string_view    next_;
};

} // detail

ARGS_NAMESPACE_END

#endif
