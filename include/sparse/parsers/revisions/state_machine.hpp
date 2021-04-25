#pragma once
#include <iostream>
#include <optional>
#include <sparse/common/utility.hpp>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace sparse::parsers::revisions
{
namespace states
{
// clang-format off
struct init {};
struct end {};

struct header_found_version {};
struct header_not_found {};

struct version_description_found {};
struct revision_date_description_found {};
struct date_version_change_found {};
struct version_date_author_change_found {};

struct in_version_description {};
struct in_revision_date_description {};
struct in_date_version_change {};
struct in_version_date_author_change {};
// clang-format on
} // namespace states

/** Allowed FSM states **/
using state_t = std::variant<states::init,
                             states::end,
                             states::header_found_version,
                             states::header_not_found,
                             states::version_description_found,
                             states::revision_date_description_found,
                             states::date_version_change_found,
                             states::version_date_author_change_found,
                             states::in_version_description,
                             states::in_revision_date_description,
                             states::in_date_version_change,
                             states::in_version_date_author_change>;

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
    static std::string get_result(const std::vector<std::string_view>& title_lines)
    {
        std::string res, sep;
        for (const auto& l : title_lines)
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
     * @param sw string to parse
     * @return document title on success, std::nullopt otherwise
     */
    static std::optional<std::string> run(std::string_view sw)
    {
        state_t state = states::init{};
        TitleTransitionFn t{sw};
        while (!std::holds_alternative<states::end>(state))
        {
            state = std::visit(t, state);
        }
        if (std::holds_alternative<states::end>(state))
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
     * @param sw string to search for title
     */
    explicit base_transition(std::string_view sw)
        : _sw{sw}
    {}

    std::vector<std::string_view> revisions; ///< Matched title lines

    /**
     * @brief Hungry template for unhandled states
     * @tparam T Any state
     * @return reject by default
     */
    template <typename T>
    state_t operator()(T)
    {
        std::cerr << "Entered unexpected state, rejecting." << std::endl;
        return states::end{};
    }

protected:
    std::string_view _sw; ///< Local copy of the string view
};

} // namespace sparse::parsers::revisions
