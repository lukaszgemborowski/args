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
        , current_ {current.substr(1)}
        , next_ {next}
    {}

    using Idx = std::make_index_sequence<std::tuple_size_v<T>>;
    template<std::size_t... I>
    using Seq = std::index_sequence<I...>;

    auto parse() -> parser_result
    {
        auto chars = opt_chars();
        auto arg_pos = current_.find_first_not_of(chars.data(), 0, chars.size());

        if (arg_pos == std::string_view::npos)
            // current_ consists only of chars -> there's no argument encoded
            return parse(Idx{});
        else
            return {false, false};
    }

private:
    constexpr auto opt_chars() const
    {
        return opt_chars_impl(Idx{});
    }


    template<std::size_t... I>
    constexpr auto opt_chars_impl(Seq<I...>) const
    {
        std::array<char, std::tuple_size_v<T>> result;
        ((
            result[I] = std::get<I>(opts_).opt_.short_name())
         , ...);

        return result;
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
