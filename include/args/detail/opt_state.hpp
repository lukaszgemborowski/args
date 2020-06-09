#ifndef ARGS_DETAIL_OPT_STATE_HPP
#define ARGS_DETAIL_OPT_STATE_HPP

#include "args/common.hpp"
#include "args/opt_parser.hpp"
#include <type_traits>

ARGS_NAMESPACE_BEGIN
namespace detail {

template<class T> // T = opt<...>
struct opt_state {
    using opt_type = typename std::remove_reference_t<T>::type;

    template<class U>
    opt_state(U&& opt)
        : opt_ {opt}
    {}

    auto parse(std::string_view current, std::string_view next)
    {
        return opt_parser<opt_type>::parse(*this, current, next);
    }

    T                                   opt_;
    typename opt_parser<opt_type>::data data_;
};

template<class U> opt_state(U &&) ->
    opt_state<U>;

} // detail
ARGS_NAMESPACE_END

#endif
