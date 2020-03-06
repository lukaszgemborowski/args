#ifndef ARGS_ARGS_HPP
#define ARGS_ARGS_HPP

#include <optional>
#include <vector>
#include <string>

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
class opt : public opt_base
{
public:
    using type_t = typename opt_traits<T>::type_t;
    using traits_t = opt_traits<T>;

    opt(char s, T &&defaultValue)
        : opt_base {s}
        , value_ {nullptr}
        , found_ {false}
        , defaultValue_ {std::move(defaultValue)}
    {}

    auto value() const
    {
        if (opt_traits<T>::has_value && value_ == nullptr)
            return std::optional<type_t>{};
        if (!found_)
            return std::optional<type_t>{};
        return opt_traits<T>::convert(value_);
    }

    T operator* () const
    {
        auto val = value();

        if (val) {
            return *val;
        } else {
            return defaultValue_;
        }
    }

    operator T () const
    {
        return *(*this);
    }

    // methods for parser use
    void set_found()
    {
        found_ = true;
    }

    void set_value(char *v)
    {
        value_ = v;
    }

private:
    char* value_;
    bool found_;
    T defaultValue_;
};

template<class T>
class assign : public opt_base
{
public:
    using type_t = typename opt_traits<T>::type_t;
    using traits_t = opt_traits<T>;

    assign(char s, T &target, T &&defaultValue = T{})
        : opt_base {s}
        , target_ {target}
        , defaultValue_ {std::move(defaultValue)}
    {}

    void set_found()
    {
        if constexpr (traits_t::has_value == false) {
            target_ = defaultValue_;
        }
    }

    void set_value(char *v)
    {
        auto value = traits_t::convert(v);
        target_ = *value;
    }

private:
    T &target_;
    T defaultValue_;
};

template<class Tuple>
class parser
{
public:
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
                call_parse_arg(argc, argv, i, std::make_index_sequence<std::tuple_size_v<Tuple>>{});
            } else {
                posargs_.push_back(argv[i]);
            }
        }
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
