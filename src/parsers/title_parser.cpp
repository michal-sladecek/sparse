#include <ctre.hpp>
#include <iostream>
#include <sparse/parsers/title_parser.hpp>
#include <sparse/parsers/tokens.hpp>
#include <string_view>
#include <variant>
#include <vector>

namespace sparse::parsers
{
std::string trim_result(const std::vector<std::string>& title_lines_)
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


class state_machine1
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
        explicit transition_t(std::string_view sw_)
            : _sw{sw_}
        {}

        std::vector<std::string> title_lines{};

        state_t operator()(init)
        {
            if (const auto [token, n_read] = tokens::try_match<tokens::newline, tokens::std_id>(_sw); token)
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
            if (const auto [token, n_read] = tokens::try_match<tokens::newline, tokens::w_for, tokens::title_line>(_sw); token)
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
            if (const auto [token, n_read] = tokens::try_match<tokens::newline, tokens::title_line>(_sw); token)
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
            if (const auto [token, n_read] = tokens::try_match<tokens::newline, tokens::title_line>(_sw); token)
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
            std::cerr << "Rejected by default" << std::endl;
            return reject{};
        }

        std::string_view _sw;
    };

public:
    static std::optional<std::string> run(std::string_view sw_)
    {
        state_t state = init{};
        transition_t t{sw_};
        while (!std::holds_alternative<state_machine1::accept>(state) && !std::holds_alternative<state_machine1::reject>(state))
        {
            state = std::visit(t, state);
        }
        if (std::holds_alternative<state_machine1::accept>(state))
        {
            return trim_result(t.title_lines);
        }
        return {};
    }
};


//###############################

/**
 * @brief State machine parses title based on 'Security Target Lite' string
 * It expects one or more title lines at the start of document, followed
 * by the string
 */
class state_machine2
{
public:
    struct init
    {};
    struct parsing_title
    {};
    struct accept
    {};
    struct reject
    {};

    using state_t = std::variant<init, parsing_title, accept, reject>;

    struct transition_t
    {
        explicit transition_t(std::string_view sw_)
            : _sw{sw_}
        {}

        std::vector<std::string> title_lines{};

        state_t operator()(init)
        {
            if (const auto [token, n_read] = tokens::try_match<tokens::w_security_traget, tokens::newline, tokens::title_line>(_sw); token)
            {
                _sw = _sw.substr(n_read);
                if (std::holds_alternative<tokens::w_security_traget>(*token))
                    return reject{};
                if (std::holds_alternative<tokens::newline>(*token))
                    return init{};
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
            if (const auto [token, n_read] = tokens::try_match<tokens::w_security_traget, tokens::newline, tokens::title_line>(_sw); token)
            {
                _sw = _sw.substr(n_read);
                if (std::holds_alternative<tokens::w_security_traget>(*token))
                    return accept{};
                if (std::holds_alternative<tokens::newline>(*token))
                    return accept{};
                if (std::holds_alternative<tokens::title_line>(*token))
                {
                    title_lines.emplace_back(std::get<tokens::title_line>(*token).matched);
                    return parsing_title{};
                }
            }
            return reject{};
        }

        template <typename T>
        state_t operator()(T)
        {
            std::cerr << "Rejected by default" << std::endl;
            return reject{};
        }

        std::string_view _sw;
    };

public:
    static std::optional<std::string> run(std::string_view sw_)
    {
        state_t state = init{};
        transition_t t{sw_};
        while (!std::holds_alternative<state_machine2::accept>(state) && !std::holds_alternative<state_machine2::reject>(state))
        {
            state = std::visit(t, state);
        }
        if (std::holds_alternative<state_machine2::accept>(state))
        {
            return trim_result(t.title_lines);
        }
        return {};
    }
};


std::optional<common::title_t> parse_title(std::string_view file_)
{
    if (const auto res = state_machine1::run(file_))
    {
        return res;
    }
    if (const auto res = state_machine2::run(file_))
    {
        return res;
    }
    return {};
}

} // namespace sparse::parsers
