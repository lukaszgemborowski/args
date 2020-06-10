#ifndef ARGS_OPT_HPP
#define ARGS_OPT_HPP

#include "args/common.hpp"
#include "args/opt_parser.hpp"
#include <string_view>
#include <type_traits>

ARGS_NAMESPACE_BEGIN

template<class T>
struct opt {
    using type = std::remove_reference_t<T>;

    opt(char s, char const* l, char const *d)
        : short_name_ {s}
        , long_name_ {l}
        , description_ {d}
        , storage_ {}
    {}

    opt(char s, char const* l, char const *d, T& t)
        : short_name_ {s}
        , long_name_ {l}
        , description_ {d}
        , storage_ {t}
    {}

    T const& value() const noexcept {
        return storage_;
    }

    T& value() noexcept {
        return storage_;
    }

    constexpr auto short_name() const noexcept {
        return short_name_;
    }

private:
    char                short_name_;
    std::string_view    long_name_;
    std::string_view    description_;
    T                   storage_;
};

template<class T> opt(char, char const*, char const*, T&) -> opt<T &>;

ARGS_NAMESPACE_END

#endif
