#include <ctre.hpp>
#include <iostream>
#include <sparse/parsers/state_machine.hpp>
#include <sparse/parsers/title_parser.hpp>
#include <sparse/parsers/tokens.hpp>
#include <string_view>
#include <variant>
#include <vector>

namespace sparse::parsers
{
namespace transitions
{
/**
 * @brief FSM transition for parsing the title
 * using common ID, e.g. 'BSI-DSZ-CC-1102-2019'
 */
struct has_standard_id : public base_transition
{
    explicit has_standard_id(std::string_view sw_)
        : base_transition{sw_}
    {}

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

    using base_transition::operator();
};

/**
 * @brief FSM transition parses title based on
 * 'Security Target Lite' string after title.
 * It expects one or more title lines at the start
 * of document, followed by 'Security Target Lite'.
 */
struct ends_with_st : public base_transition
{
    explicit ends_with_st(std::string_view sw_)
        : base_transition{sw_}
    {}

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
                return finished_title{};
            if (std::holds_alternative<tokens::title_line>(*token))
            {
                title_lines.emplace_back(std::get<tokens::title_line>(*token).matched);
                return parsing_title{};
            }
        }
        return reject{};
    }

    state_t operator()(finished_title)
    {
        if (const auto [token, n_read] = tokens::try_match<tokens::newline, tokens::w_security_traget>(_sw); token)
        {
            _sw = _sw.substr(n_read);
            if (std::holds_alternative<tokens::w_security_traget>(*token))
                return accept{};
            if (std::holds_alternative<tokens::newline>(*token))
                return finished_title{};
        }
        return reject{};
    }

    using base_transition::operator();
};

/**
 * @brief FSM transition parses title using
 * version identifier, e.g. 'Version 2020-4'
 */
struct version : public base_transition
{
    explicit version(std::string_view sw_)
        : base_transition{sw_}
    {}

    state_t operator()(init)
    {
        const auto [token, n_read] = tokens::try_match<tokens::w_version>(_sw);
        if (token)
        {
            _sw = _sw.substr(n_read);
            if (std::holds_alternative<tokens::w_version>(*token))
                return got_version{};
        }
        return reject{};
    }

    state_t operator()(got_version)
    {
        if (const auto [token, n_read] = tokens::try_match<tokens::newline, tokens::title_line>(_sw); token)
        {
            _sw = _sw.substr(n_read);
            if (std::holds_alternative<tokens::newline>(*token))
                return got_version{};
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
        return reject{};
    }

    state_t operator()(finished_title)
    {
        if (const auto [token, n_read] = tokens::try_match<tokens::newline, tokens::w_security_traget>(_sw); token)
        {
            _sw = _sw.substr(n_read);
            if (std::holds_alternative<tokens::w_security_traget>(*token))
                return accept{};
            if (std::holds_alternative<tokens::newline>(*token))
                return finished_title{};
        }
        return reject{};
    }

    using base_transition::operator();
};

/**
 * @brief FSM transition parses title based on
 * 'Security Target Lite' string before title.
 */
struct starts_with_st : public base_transition
{
    explicit starts_with_st(std::string_view sw_)
        : base_transition{sw_}
    {}

    state_t operator()(init)
    {
        if (const auto [token, n_read] = tokens::try_match<tokens::w_security_traget, tokens::newline>(_sw); token)
        {
            _sw = _sw.substr(n_read);
            if (std::holds_alternative<tokens::w_security_traget>(*token))
                return got_security_target{};
            if (std::holds_alternative<tokens::newline>(*token))
                return init{};
        }
        return reject{};
    }

    state_t operator()(got_security_target)
    {
        if (const auto [token, n_read] = tokens::try_match<tokens::title_line, tokens::newline>(_sw); token)
        {
            _sw = _sw.substr(n_read);
            if (std::holds_alternative<tokens::newline>(*token))
                return got_security_target{};
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

    using base_transition::operator();
};
} // namespace transitions

std::optional<common::title_t> parse_title(std::string_view file_)
{
    using parse_by_id                  = state_machine<transitions::has_standard_id>;
    using parse_by_sec_target_lite     = state_machine<transitions::ends_with_st>;
    using parse_by_version             = state_machine<transitions::version>;
    using parse_begins_with_sec_target = state_machine<transitions::starts_with_st>;

    if (const auto res = parse_by_id::run(file_))
        return res;
    if (const auto res = parse_by_sec_target_lite::run(file_))
        return res;
    if (const auto res = parse_by_version::run(file_))
        return res;
    if (const auto res = parse_begins_with_sec_target::run(file_))
        return res;

    return {};
}

} // namespace sparse::parsers
