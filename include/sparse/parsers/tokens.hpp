#pragma once

#include <ctre.hpp>
#include <optional>
#include <string_view>
#include <tuple>
#include <variant>

namespace sparse::parsers::title::tokens
{
namespace detail
{
/**
 * @tparam Ret Return type
 * @tparam First First token
 * @tparam Rest Rest of the tokens
 * @param sw_ string view
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
     * @param sw_ string to match
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

struct newline : public token<newline>
{
    static constexpr ctll::fixed_string re = "\\h*\\n"; ///< Sequence of whitespace characters and '\n'
};
struct common_id : public token<common_id>
{
    static constexpr ctll::fixed_string re = "\\h*BSI-DSZ-CC(-[A-Z0-9]{1,5})+\\n"; ///< ID commonly used in reports
};
struct word_for : public token<word_for>
{
    static constexpr ctll::fixed_string re = "\\h*for\\n"; ///< Line containing 'for' keyword
};
struct title_line : public token<title_line>
{
    static constexpr ctll::fixed_string re = "\\h*([^\\n]+)\\n"; ///< Single line of title, can be anything
};
struct word_from : public token<word_from>
{
    static constexpr ctll::fixed_string re = "\\h*from\\n"; ///< Line containing 'from' keyword
};

/**
 * @brief Matches either 'Security Target Lite' or 'SECURITY TARGET LITE'
 * the 'Lite' word is optional in both cases
 */
struct word_security_traget : public token<word_security_traget>
{
    static constexpr ctll::fixed_string re = "\\h*(Security Target|SECURITY TARGET)( Lite| LITE)?\\h*\\n";
};

/**
 * @brief Matches anythin until it finds Version
 * There is hard limit on characters before Version to assure that it's
 * found somewhere in the beginning
 * Example: 'bla bla\n bla Version 2020-4\n'
 */
struct word_version : public token<word_version>
{
    static constexpr ctll::fixed_string re = ".{0,1000}Version \\d{4}-\\d\\n";
};

/** variant which can hold any of the tokens **/
using tokens = std::variant<newline, common_id, word_for, word_from, title_line, word_security_traget, word_version>;

} // namespace sparse::parsers::title::tokens
