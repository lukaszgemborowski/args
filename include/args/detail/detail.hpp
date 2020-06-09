#ifndef ARGS_DETAIL_DETAIL_HPP
#define ARGS_DETAIL_DETAIL_HPP

#include "args/common.hpp"

ARGS_NAMESPACE_BEGIN
namespace detail {

inline auto is_short = [](char *arg) -> bool {
    return arg[0] == '-' && arg[1] != '-';
};

inline auto is_long = [](char *arg) -> bool {
    return arg[0] == '-' && arg[1] == '-' &&
            arg[2] != '-';
};

} // namespace detail
ARGS_NAMESPACE_END

#endif
