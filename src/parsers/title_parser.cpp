#include <ctre.hpp>
#include <iostream>
#include <sparse/parsers/title_parser.hpp>
#include <sparse/parsers/tokens.hpp>
#include <string_view>
#include <variant>
#include <vector>

namespace sparse::parsers
{
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

std::optional<common::title_t> parse_title(std::string_view file_)
{
    state_machine sm1{};
    if (const auto res = sm1.run(file_))
    {
        return res;
    }
    return {};
}

} // namespace sparse::parsers
