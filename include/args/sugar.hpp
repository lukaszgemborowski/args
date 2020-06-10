#ifndef ARGS_SUGAR_HPP
#define ARGS_SUGAR_HPP

#include "args/common.hpp"
#include "args/opt.hpp"

ARGS_NAMESPACE_BEGIN

constexpr basic_opt operator"" _o (char s)
{
    return basic_opt{s};
}

template<class T>
struct of_type {};

template<class T>
auto operator | (basic_opt &&o, of_type<T>)
{
    return opt<T>{o};
}

ARGS_NAMESPACE_END

#endif
