#pragma once
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


namespace tokens
{
struct header : public token<header>
{
    static constexpr ctll::fixed_string re = "^(.|\\n|\\n\\r)*(Revision History|Version Control)";
};

struct version_description_header : public token<version_description_header>
{
    static constexpr ctll::fixed_string re = "^(.|\\n|\\n\\r)*Version(\\h*)+Description of change";
};

struct revision_date_description_header : public token<revision_date_description_header>
{
    static constexpr ctll::fixed_string re = "^(.|\\n|\\n\\r)*(Rev|Revision|Version)(\\h*)+(Date|Release Date)(\\h*)+(Description number|Description)(\\h*)+";
};

struct date_version_change_header : public token<date_version_change_header>
{
    static constexpr ctll::fixed_string re = "^(.|\\n|\\n\\r)*Date(\\h*)+Version(\\h*)+Change notice(\\h*)+";
};

struct version_date_author_changes_header : public token<version_date_author_changes_header>
{
    static constexpr ctll::fixed_string re = "^(.|\n|\n\r)*Version(\\h*)+Date(\\h*)+Author(\\h*)+Changes to Previous version";
};

struct version : public token<version>
{
    static constexpr ctll::fixed_string re = "(v|Rev\\. |Version )?\\d.\\d";
};

struct date : public token<date>
{
    static constexpr ctll::fixed_string re = "(\\d{4}-\\d{1,2}-\\d{1,2}|\\d{1,2}.\\d{1,2}.\\d{4}|\\d{1,2}[ "
                                             "-](January|February|March|April|May|June|July|August|September|October|November|December)[ -]\\d{4})";
};

struct author : public token<author>
{
    static constexpr ctll::fixed_string re = "[a-zA-Z]+ [a-zA-Z]+ ([a- dzA-Z]+)?";
};

struct description : public token<description>
{
    static constexpr ctll::fixed_string re = ".*(\\r\\n|\\n)";
};

// Unfortunately it is not possible to concatenate ctll::fixed_string
struct version_description : public token<version_description>
{
    static constexpr ctll::fixed_string re = "(v|Rev\\. |Version )?\\d.\\d[\\h]"
                                             "*.*(\\r\\n|\\n)";
};

struct revision_date_description : public token<revision_date_description>
{
    static constexpr ctll::fixed_string re = "(v|Rev\\. |Version )?\\d.\\d[\\h]*"
                                             "(\\d{4}-\\d{1,2}-\\d{1,2}|\\d{1,2}.\\d{1,2}.\\d{4}|\\d{1,2}[-](January|February|March|April|May|"
                                             "June|July|August|September|October|November|December)[ -]\\d{4})[\\h]"
                                             "*.*(\\r\\n|\\n)";
};

struct date_version_change : public token<date_version_change>
{
    static constexpr ctll::fixed_string re = "(\\d{4}-\\d{1,2}-\\d{1,2}|\\d{1,2}.\\d{1,2}.\\d{4}|\\d{1,2}[ "
                                             "-](January|February|March|April|May|June|July|August|September|October|November|December)[ -]\\d{4})"
                                             "(v|Rev\\. |Version )?\\d.\\d"
                                             ".*(\\r\\n|\\n)";
};

struct version_date_author_changes : public token<version_date_author_changes>
{
    static constexpr ctll::fixed_string re = "(\\d{4}-\\d{1,2}-\\d{1,2}|\\d{1,2}.\\d{1,2}.\\d{4}|\\d{1,2}[ "
                                             "-](January|February|March|April|May|June|July|August|September|October|November|December)[ -]\\d{4})"
                                             "(v|Rev\\. |Version )?\\d.\\d"
                                             "[a-zA-Z]+ [a-zA-Z]+ ([a- dzA-Z]+)?"
                                             ".*(\\r\\n|\\n)";
};
} // namespace tokens
} // namespace sparse::parsers::revisions
