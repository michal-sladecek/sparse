#pragma once

#include <ctre.hpp>
#include <optional>
#include <string_view>
#include <tuple>
#include <variant>

namespace sparse::parsers::tokens
{
namespace detail
{
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
} // namespace detail


template <typename First, typename... Rest, typename Ret = std::pair<std::optional<std::variant<First, Rest...>>, size_t>>
Ret try_match(std::string_view ss)
{
    return detail::_try_match_tokens<Ret, First, Rest...>(ss);
}


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
} // namespace sparse::parsers::tokens
