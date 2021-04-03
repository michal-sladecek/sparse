#include <ctre.hpp>
#include <iostream>
#include <sparse/parsers/title_parser.hpp>
#include <string_view>
#include <variant>
#include <vector>

namespace sparse::parsers
{
namespace tokens
{
template <typename T>
struct token
{
    bool match(std::string_view s)
    {
        if (const auto res = ctre::starts_with<T::re>(s))
        {
            matched = res.to_view();
            return true;
        }
        return false;
    }
    std::string_view matched;
};

struct newline : public token<newline>
{
    static constexpr ctll::fixed_string re = "\\h*\\n";
};
struct std_id : public token<std_id>
{
    static constexpr ctll::fixed_string re = "\\h*BSI-DSZ-CC(-[A-Z0-9]{1,5})+\\n";
};
struct w_for : public token<w_for>
{
    static constexpr ctll::fixed_string re = "\\h*for\\n";
};
struct title_line : public token<title_line>
{
    static constexpr ctll::fixed_string re = "\\h*([^\\n]+)\\n";
};
struct w_from : public token<w_from>
{
    static constexpr ctll::fixed_string re = "\\h*from\\n";
};

using tokens = std::variant<newline, std_id, w_for, w_from, title_line>;
} // namespace tokens


template <typename Ret, typename First, typename... Rest>
Ret _try_match_tokens(std::string_view ss)
{
    First t{};
    if (t.match(ss))
    {
        return {t, t.matched.size()};
    }
    if constexpr (sizeof...(Rest) > 0)
    {
        return _try_match_tokens<Ret, Rest...>(ss);
    }
    return {std::nullopt, 0};
}


template <typename First, typename... Rest, typename Ret = std::pair<std::optional<std::variant<First, Rest...>>, size_t>>
Ret try_match_tokens(std::string_view ss)
{
    return _try_match_tokens<Ret, First, Rest...>(ss);
}

class state_machine
{
public:
    struct init
    {};
    struct got_id
    {};
    struct got_for
    {};
    struct accept
    {};
    struct parsing_title
    {};
    struct reject
    {};


    using state_t = std::variant<init, got_id, got_for, accept, parsing_title, reject>;

    struct transition_t
    {
        transition_t(std::string_view sw_)
            : _sw{sw_}
        {}

        std::vector<std::string> title_lines{};

        state_t operator()(init)
        {
            if (const auto [token, n_read] = try_match_tokens<tokens::newline, tokens::std_id>(_sw); token)
            {
                _sw = _sw.substr(n_read);
                if (std::holds_alternative<tokens::newline>(*token))
                    return init{};
                if (std::holds_alternative<tokens::std_id>(*token))
                    return got_id{};
            }
            return reject{};
        }

        state_t operator()(got_id)
        {
            if (const auto [token, n_read] = try_match_tokens<tokens::newline, tokens::w_for, tokens::title_line>(_sw); token)
            {
                _sw = _sw.substr(n_read);
                if (std::holds_alternative<tokens::newline>(*token))
                    return got_id{};
                if (std::holds_alternative<tokens::w_for>(*token))
                    return got_for{};
                if (std::holds_alternative<tokens::title_line>(*token))
                {
                    title_lines.emplace_back(std::get<tokens::title_line>(*token).matched);
                    return parsing_title{};
                }
            }
            return reject{};
        }

        state_t operator()(got_for)
        {
            if (const auto [token, n_read] = try_match_tokens<tokens::newline, tokens::title_line>(_sw); token)
            {
                _sw = _sw.substr(n_read);
                if (std::holds_alternative<tokens::newline>(*token))
                    return got_for{};
                if (std::holds_alternative<tokens::title_line>(*token))
                {
                    title_lines.emplace_back(std::get<tokens::title_line>(*token).matched);
                    return parsing_title{};
                }
            }
            return reject{};
        }


        state_t operator()(parsing_title)
        {
            if (const auto [token, n_read] = try_match_tokens<tokens::newline, tokens::title_line>(_sw); token)
            {
                _sw = _sw.substr(n_read);
                if (std::holds_alternative<tokens::newline>(*token))
                    return accept{};
                if (std::holds_alternative<tokens::title_line>(*token))
                {
                    title_lines.emplace_back(std::get<tokens::title_line>(*token).matched);
                    return parsing_title{};
                }
            }
            return accept{};
        }


        template <typename T>
        state_t operator()(T)
        {
            std::cout << "Rejected by default" << std::endl;
            return reject{};
        }

        std::string_view _sw;
    };

public:
    std::optional<std::string> run(std::string_view sw_) const
    {
        state_t state = init{};
        transition_t t{sw_};
        while (!std::holds_alternative<state_machine::accept>(state) && !std::holds_alternative<state_machine::reject>(state))
        {
            state = std::visit(t, state);
        }
        if (std::holds_alternative<state_machine::accept>(state))
        {
            return get_result(t.title_lines);
        }
        return {};
    }

    std::string get_result(const std::vector<std::string>& title_lines_) const
    {
        static constexpr ctll::fixed_string trim_regex{"\\h*([^\\n]*)\\n"};
        std::string res;
        std::string sep;
        for (const auto& l : title_lines_)
        {
            res += sep;
            res += ctre::match<trim_regex>(l).get<1>();
            sep = " ";
        }
        return res;
    }
};


class bib_parser
{
public:
    bib_parser(std::string_view file_)
        : _file{file_}
    {}
    auto run()
    {
        state_machine sm{};
        return sm.run(_file);
    }

private:
    std::optional<tokens::tokens> get_token() { using namespace tokens; }
    std::string_view _file;
    size_t iter{};
    bool finished{};
};


std::optional<common::title_t> parse_title(std::string_view file_)
{
    bib_parser parser{file_};
    return parser.run();
    return {};
}

} // namespace sparse::parsers
