#ifndef ARGS_DETAIL_POSITIONAL_PARSER_HPP
#define ARGS_DETAIL_POSITIONAL_PARSER_HPP

#include "args/common.hpp"
#include "args/parser_result.hpp"
#include <string_view>

ARGS_NAMESPACE_BEGIN
namespace detail {

template<class T>
struct positional_parser
{
    positional_parser(T& opts, std::string_view current, std::string_view next)
        : opts_ {opts}
        , current_ {current}
        , next_ {next}
    {}

    auto parse() -> parser_result
    {
    }

private:
    T&                  opts_;
    std::string_view    current_;
    std::string_view    next_;
};

} // namespace detail
ARGS_NAMESPACE_END

#endif
