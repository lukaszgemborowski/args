#ifndef ARGS_TESTS_HELPERS_HPP
#define ARGS_TESTS_HELPERS_HPP

struct CreateArgs
{
    CreateArgs(std::initializer_list<std::string> a)
    {
        args_.push_back({});

        for (auto v : a) {
            std::vector<char> current{
                v.begin(), v.end()};
            current.push_back(0);
            args_.push_back(current);
        }

        for (auto &v : args_) {
            argv_.push_back(&v[0]);
        }
    }

    int argc() const
    {
        return args_.size();
    }

    char** argv()
    {
        return &argv_[0];
    }


private:
    std::vector<char *> argv_;
    std::vector<std::vector<char>> args_;
};

#endif
