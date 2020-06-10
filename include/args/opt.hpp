#ifndef ARGS_OPT_HPP
#define ARGS_OPT_HPP

#include "args/common.hpp"
#include "args/opt_parser.hpp"
#include <string_view>
#include <type_traits>

ARGS_NAMESPACE_BEGIN

struct description
{
    explicit description(char const* value)
        : value_ {value}
    {}

    char const* value_;
};

struct basic_opt
{
    basic_opt(basic_opt const& rhs) = default;

    constexpr basic_opt(char s, char const* l, char const *d)
        : short_name_ {s}
        , long_name_ {l}
        , description_ {d}
    {}

    constexpr basic_opt(char s)
        : short_name_ {s}
    {}

    constexpr auto short_name() const noexcept {
        return short_name_;
    }

    constexpr auto description() const noexcept {
        return description_;
    }

    auto operator|(class description &&desc) &&
    {
        description_ = desc.value_;
        return *this;
    }

private:
    char                short_name_;
    std::string_view    long_name_ = {};
    std::string_view    description_ = {};
};

template<class T>
struct opt : basic_opt {
    using type = std::remove_reference_t<T>;
    using basic_opt::basic_opt;

    opt(basic_opt const& rhs)
        : basic_opt{rhs}
    {}

    opt(char s, char const* l, char const *d, T& t)
        : basic_opt {s, l, d}
        , storage_ {t}
    {}

    opt(char s, T& t)
        : basic_opt{s}
        , storage_ {t}
    {}

    T const& value() const noexcept {
        return storage_;
    }

    T& value() noexcept {
        return storage_;
    }

private:
    T                   storage_ = {};
};

template<class T> opt(char, char const*, char const*, T&) -> opt<T &>;
template<class T> opt(char, T&) -> opt<T &>;

ARGS_NAMESPACE_END

#endif
