#ifndef ARGS_ARGS_HPP
#define ARGS_ARGS_HPP

#include <optional>
#include <vector>
#include <string>
#include <cstring>

namespace args
{

class opt_base
{
public:
    opt_base(char s)
        : short_ {s}
    {}

    auto short_name() const
    {
        return short_;
    }

protected:
    char short_;
};

template<class T> struct opt_traits;

template<>
struct opt_traits<void>
{
    using type_t = bool;
    static constexpr auto has_value = false;
    static std::optional<type_t> convert(const char *ptr)
    {
        return true;
    }
};

template<>
struct opt_traits<int>
{
    using type_t = int;
    static constexpr auto has_value = true;
    static std::optional<int> convert(const char *ptr)
    {
        char *str_end = nullptr;
        auto len = std::strlen(ptr);
        auto result = std::strtol(ptr, &str_end, 10);

        if ((str_end - ptr) < len) {
            return {};
        } else {
            return {result};
        }
    }
};

template<>
struct opt_traits<std::string>
{
    using type_t = std::string;
    static constexpr auto has_value = true;
    static std::optional<std::string> convert(const char *ptr)
    {
        return {ptr};
    }
};

template<>
struct opt_traits<bool>
{
    using type_t = bool;
    static constexpr auto has_value = false;
    static std::optional<bool> convert(const char *)
    {
        return true;
    }
};

template<class T>
struct save
{
    save(T &target)
        : target {target}
    {}

    T &target;
};

template<class T>
struct identity
{
    T operator()(const T &input) const
    {
        return input;
    }
};

template<class T, class Converter = identity<T>>
class opt : public opt_base
{
public:
    using target_t = decltype(std::declval<Converter>()(std::declval<T>()));
    static_assert(std::is_reference_v<target_t> == false);

    using traits_t = opt_traits<T>;

    opt(char s, T defaultValue)
        : opt_base {s}
        , hasDefault_ {true}
        , defaultValue_ {defaultValue}
    {}

    opt(char s, save<target_t> &&target)
        : opt_base {s}
    {
        save(target.target);
    }

    opt(char s, save<target_t> &&target, T defaultValue)
        : opt_base {s}
        , hasDefault_ {true}
        , defaultValue_ {defaultValue}
    {
        save(target.target);
    }

    explicit opt(char s)
        : opt_base {s}
    {}

    auto& save(target_t &target)
    {
        target_ = &target;
        return *this;
    }

    auto& value() const
    {
        const target_t &t = target_ ? *target_ : result_;
        return t;
    }

    // methods for parser use
    void set_found()
    {
        if constexpr (traits_t::has_value == false) {
            found_ = true;
            target_t &t = target_ ? *target_ : result_;

            if (hasDefault_) {
                t = converter_(defaultValue_);
            } else {
                auto arg = opt_traits<T>::convert(nullptr);
                t = converter_(arg);
            }
        }
    }

    bool found() const
    {
        return found_;
    }

    void set_value(const char *v)
    {
        auto arg = opt_traits<T>::convert(v);

        if (!arg)
            return;

        found_ = true;

        if (target_) {
            *target_ = converter_(*arg);
        } else {
            result_ = converter_(*arg);
        }
    }

    void parse_complete()
    {
        target_t &t = target_ ? *target_ : result_;

        if (!found_ && hasDefault_) {
            t = converter_(defaultValue_);
        }
    }

private:
    bool        found_ = false;
    target_t*   target_ = nullptr;
    target_t    result_ = target_t {};
    bool        hasDefault_ = false;
    T           defaultValue_ = T{};
    Converter   converter_ = Converter{};
};

template<class T>
opt(char, T) -> opt<T, identity<T>>;

template<class T>
opt(char, save<T> &&) -> opt<T, identity<T>>;

template<class Tuple>
class parser
{
public:
    static constexpr auto options_count = std::tuple_size_v<Tuple>;

    template<class... Opts>
    parser(Opts&&... args)
        : opts_ {std::forward_as_tuple(std::forward<Opts>(args)...)}
        , posargs_ {}
    {}

    void parse(int argc, char **argv)
    {
        bool prevWasArg = false;
        for (auto i = 1; i < argc; ++i) {
            if (argv[i][0] == '-') {
                call_parse_arg(argc, argv, i, std::make_index_sequence<options_count>{});
            } else {
                posargs_.push_back(argv[i]);
            }
        }

        finish(std::make_index_sequence<options_count>{});
    }

    const auto& posargs() const
    {
        return posargs_;
    }

    auto& posargs()
    {
        return posargs_;
    }

private:
    template<std::size_t... I>
    void finish(std::index_sequence<I...>)
    {
        (std::get<I>(opts_).parse_complete(), ...);
    }

    template<std::size_t... I>
    void call_parse_arg(int argc, char **argv, int &index, std::index_sequence<I...>)
    {
        bool argumentConsumed = false;
        (parse_arg<I>(argc, argv, index, argumentConsumed), ...);

        if (argumentConsumed)
            ++ index;
    }

    template<std::size_t I>
    void parse_arg(int argc, char **argv, int index, bool &argumentConsumed)
    {
        auto &o = std::get<I>(opts_);

        for (auto ptr = argv[index];
             *ptr != '\0';
             ptr ++) {
            auto c = *ptr;

            if (c != o.short_name())
                continue;

            o.set_found();

            if (!std::decay_t<std::tuple_element_t<I, decltype(opts_)>>::traits_t::has_value)
                continue;

            if ((index + 1) < argc) {
                o.set_value(argv[index + 1]);
                argumentConsumed = true;
            }
        }
    }

private:
    Tuple opts_;
    std::vector<char *> posargs_;
};

template<class... Opts> parser(Opts&&... args) -> parser<decltype(std::forward_as_tuple(std::forward<Opts>(args)...))>;

} // namespace args

#endif // ARGS_ARGS_HPP
