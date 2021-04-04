#pragma once
#include <ctre.hpp>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace sparse::parsers
{
inline namespace states
{
// clang-format off
struct init {};
struct accept {};
struct reject {};
struct got_id {};
struct got_for {};
struct parsing_title {};
struct finished_title {};
struct got_version {};
struct got_security_target{};
// clang-format on
} // namespace states

using state_t = std::variant<init, accept, reject, got_id, got_for, parsing_title, finished_title, got_version, got_security_target>;

template <typename TitleTransitionFn>
class state_machine
{
    static std::string trim_result(const std::vector<std::string>& title_lines_)
    {
        static constexpr ctll::fixed_string trim_regex{"\\h*([^\\n]*)\\n"};
        std::string res;
        std::string sep;
        for (const auto& l : title_lines_)
        {
            res += sep;
            res += ctre::match<trim_regex>(l).template get<1>();
            sep = " ";
        }
        return res;
    }

public:
    static std::optional<std::string> run(std::string_view sw_)
    {
        state_t state = init{};
        TitleTransitionFn t{sw_};
        while (!std::holds_alternative<accept>(state) && !std::holds_alternative<reject>(state))
        {
            state = std::visit(t, state);
        }
        if (std::holds_alternative<accept>(state))
        {
            return trim_result(t.title_lines);
        }
        return {};
    }
};

class base_transition
{
public:
    explicit base_transition(std::string_view sw_)
        : _sw{sw_}
    {}

    std::vector<std::string> title_lines{};

    template <typename T>
    state_t operator()(T)
    {
        std::cerr << "Entered unexpected state, rejecting." << std::endl;
        return reject{};
    }

protected:
    std::string_view _sw;
};

} // namespace sparse::parsers
