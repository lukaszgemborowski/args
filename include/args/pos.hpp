#ifndef ARGS_POS_HPP
#define ARGS_POS_HPP

#include "args/common.hpp"
#include <string_view>

ARGS_NAMESPACE_BEGIN

struct basic_pos
{
private:
    std::string_view name_;
    std::string_view description_;
};

template<class T>
struct pos : basic_pos
{
private:
    T storage_ = {};
};

ARGS_NAMESPACE_END

#endif
