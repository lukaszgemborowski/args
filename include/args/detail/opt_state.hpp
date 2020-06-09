#ifndef ARGS_DETAIL_OPT_STATE_HPP
#define ARGS_DETAIL_OPT_STATE_HPP

#include "args/common.hpp"
#include "args/opt_parser.hpp"

ARGS_NAMESPACE_BEGIN
namespace detail {

template<class T>
struct opt_state {
    template<class U>
    opt_state(U&& opt)
        : opt_ {opt}
    {}

    auto parse(int c, int argc, char **argv)
    {
        return opt_parser<T>::parse(*this, c, argc, argv);
    }

    T opt_;
    typename opt_parser<T>::data data_;
};

template<class U> opt_state(U &&) ->
    opt_state<U>;

} // detail
ARGS_NAMESPACE_END

#endif
