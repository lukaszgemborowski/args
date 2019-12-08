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
        , value_ {nullptr}
        , found_ {false}
    {}

    auto short_name() const
    {
        return short_;
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

protected:
    char short_;
    char* value_;
    bool found_;
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
    static constexpr auto has_value = true;
    static std::optional<bool> convert(const char *ptr)
    {
        auto r = opt_traits<int>::convert(ptr);

        if (r) {
            return {*r};
        }

        return {};
    }
};

template<class T>
class opt : public opt_base
{
public:
    using type_t = typename opt_traits<T>::type_t;

    explicit opt(char s)
        : opt_base {s}
    {}

    auto value() const
    {
        if (opt_traits<T>::has_value && value_ == nullptr)
            return std::optional<type_t>{};
        if (!found_)
            return std::optional<type_t>{};
        return opt_traits<T>::convert(value_);
    }

private:
};

template<class... Args>
class parser
{
public:
    parser(Args&... args)
        : opts_ {args...}
    {}

    void parse(int argc, char **argv)
    {
        for (auto i = 1; i < argc; ++i) {
            if (argv[i][0] == '-') {
                call_parse_arg(argc, argv, i, std::make_index_sequence<sizeof... (Args)>{});
            } else {
                posargs_.push_back(argv[i]);
            }
        }
    }

private:
    template<std::size_t... I>
    void call_parse_arg(int argc, char **argv, int index, std::index_sequence<I...>)
    {
        (parse_arg<I>(argc, argv, index), ...);
    }

    template<std::size_t I>
    void parse_arg(int argc, char **argv, int index)
    {
        auto &o = std::get<I>(opts_);

        for (auto ptr = argv[index];
             *ptr != '\0';
             ptr ++) {
            auto c = *ptr;

            if (c != o.short_name())
                continue;

            o.set_found();

            if ((index + 1) < argc) {
                o.set_value(argv[index + 1]);
            }
        }
    }

private:
    std::tuple<Args&...> opts_;
    std::vector<char *> posargs_;
};

} // namespace args

#endif // ARGS_ARGS_HPP
