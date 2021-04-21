#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <ctre.hpp>
#include <optional>
#include <string_view>
#include <tuple>
#include <variant>

namespace sparse::parsers::revisions
{
namespace detail
{
/**
 * @tparam Ret Return type
 * @tparam First First token
 * @tparam Rest Rest of the tokens
 * @param sw string view
 * @return token with match and matched size
 */
template <typename Ret, typename First, typename... Rest>
Ret try_match_tokens(std::string_view sw)
{
    First t{};
    if (t.match(sw))
    {
        return {t, t.matched.size()};
    }
    if constexpr (sizeof...(Rest) > 0)
    {
        return try_match_tokens<Ret, Rest...>(sw);
    }
    return {std::nullopt, 0};
}
} // namespace detail

/**
 * @brief Tries to match given string with token regexes
 * @return optional with matched token and size of matched substring, 0 if no match is found
 */
template <typename First, typename... Rest, typename Ret = std::pair<std::optional<std::variant<First, Rest...>>, size_t>>
Ret try_match(std::string_view ss)
{
    return detail::try_match_tokens<Ret, First, Rest...>(ss);
}

/**
 * @brief Base class for token, provides match method
 * @tparam T type of token subclass
 */
template <typename T>
struct token
{
    /**
     * @brief Tries to match given string with token regex
     * Stores the match if successful
     * @param sw string to match
     * @return true if matched
     */
    bool match(std::string_view sw)
    {
        if (const auto res = ctre::starts_with<T::re>(sw))
        {
            matched = res.to_view();
            return true;
        }
        return false;
    }
    std::string_view matched; ///< Mached substring
};

// ToDo: add regular expressions
struct header : public token<header>
{
    // static constexpr ctll::fixed_string re = "\\h*\\n"; ///< Sequence of whitespace characters and Revision History or Version Control
};

struct version_description : public token<version_description>
{
    // static constexpr ctll::fixed_string re = "\\h*\\n"; ///< Sequence of whitespace characters and Revision History or Version Control
};

struct revision_date_description : public token<revision_date_description>
{
    // static constexpr ctll::fixed_string re = "\\h*\\n"; ///< Sequence of whitespace characters and Revision History or Version Control
};

struct date_version_change : public token<date_version_change>
{
    // static constexpr ctll::fixed_string re = "\\h*\\n"; ///< Sequence of whitespace characters and Revision History or Version Control
};

struct version_date_author_changes : public token<version_date_author_changes>
{
    // static constexpr ctll::fixed_string re = "\\h*\\n"; ///< Sequence of whitespace characters and Revision History or Version Control
};

} // namespace sparse::parsers::revisions
#endif // TOKENS_HPP
