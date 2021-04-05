#pragma once
#include <iostream>
#include <optional>
#include <sparse/common/utility.hpp>
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

/** Allowed FSM states **/
using state_t = std::variant<init, accept, reject, got_id, got_for, parsing_title, finished_title, got_version, got_security_target>;

/**
 * @tparam TitleTransitionFn Functor which defines valid transitions
 * @brief State machine for parsing title
 *
 * A state_machine run starts in <init> state, the call operators of
 * TitleTransitionFn are invoked repeatedly, updating state, until
 * <accept> or <reject> state is reached. If run ended in <accept>
 * state, result title lines are trimmed and returned
 */
template <typename TitleTransitionFn>
class state_machine
{
    /**
     * @brief Concatenates result lines into single string
     * @param title_lines_ lines matched during FSM execution
     * @return std::string with title
     */
    static std::string get_result(const std::vector<std::string_view>& title_lines_)
    {
        std::string res, sep;
        for (const auto& l : title_lines_)
        {
            res += sep;
            res += common::trim_line(l);
            sep = " ";
        }
        return res;
    }

public:
    /**
     * @brief Runs the state machine, stops when <accept> or <reject> state is reached
     * @param sw_ string to parse
     * @return document title on success, std::nullopt otherwise
     */
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
            return get_result(t.title_lines);
        }
        return {};
    }
};

/**
 * @brief Base class for all FSM transition functors
 *
 * Child classes should implement a set of overloaded call operators,
 * which take <state> as an argument, and return next
 * state stored in 'state_t'. These operators are responsible for
 * updating '_sw' and 'title_lines' members.
 *
 * Example:
 *   state_t operator()(init){ return accept{}; }
 * Will provide state machine with simple transition function
 * <init> -> { <accept> }
 *
 * Class also providies default transition for any unhandled state T:
 * <T> -> { <reject> }
 */
class base_transition
{
public:
    /**
     * @brief Constructor stores the string view, it gets
     * changed during the execution
     * @param sw_ string to search for title
     */
    explicit base_transition(std::string_view sw_)
        : _sw{sw_}
    {}

    std::vector<std::string_view> title_lines{}; ///< Matched title lines

    /**
     * @brief Hungry template for unhandled states
     * @tparam T Any state
     * @return reject by default
     */
    template <typename T>
    state_t operator()(T)
    {
        std::cerr << "Entered unexpected state, rejecting." << std::endl;
        return reject{};
    }

protected:
    std::string_view _sw; ///< Local copy of the string view
};

} // namespace sparse::parsers
