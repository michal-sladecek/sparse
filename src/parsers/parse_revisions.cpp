#include <sparse/parsers/parsers.hpp>
#include <sparse/parsers/revisions/state_machine.hpp>
#include <sparse/parsers/revisions/tokens.hpp>

namespace sparse::parsers
{
namespace revisions::transitions
{
struct has_revision_history : revisions::base_transition
{
    explicit has_revision_history(std::string_view sw)
        : base_transition{sw}
    {}

    revisions::state_t operator()(states::init)
    {
        if (const auto [token, n_read] = try_match<revisions::tokens::header>(_sw); token)
        {
            _sw = _sw.substr(n_read);
            return revisions::states::header_found_version{};
        }
        return revisions::states::header_not_found{};
    }

    revisions::state_t operator()(states::header_found_version)
    {
        using namespace tokens;
        if (const auto [token, n_read] =
                try_match<version_description_header, revision_date_description_header, date_version_change_header, version_date_author_changes_header>(_sw);
            token)
        {
            _sw = _sw.substr(n_read);
            if (std::holds_alternative<version_description_header>(*token))
                return states::version_description_found{};
            if (std::holds_alternative<revision_date_description_header>(*token))
                return states::revision_date_description_found{};
            if (std::holds_alternative<date_version_change_header>(*token))
                return states::date_version_change_found{};
            if (std::holds_alternative<version_date_author_changes_header>(*token))
                return states::version_date_author_change_found{};
        }
        return states::end{};
    }

    revisions::state_t operator()(states::version_description_found)
    {
        if (const auto [token, n_read] = try_match<tokens::date>(_sw); token)
        {
            revisions.emplace_back(std::get<tokens::date>(*token).matched);
            _sw = _sw.substr(n_read);
            return states::in_version_description{};
        }
        return states::end{};
    }

    revisions::state_t operator()(states::revision_date_description_found)
    {
        if (const auto [token, n_read] = try_match<tokens::revision_date_description>(_sw); token)
        {
            revisions.emplace_back(std::get<tokens::revision_date_description>(*token).matched);
            _sw = _sw.substr(n_read);
            return states::in_revision_date_description{};
        }
        return states::end{};
    }

    revisions::state_t operator()(states::date_version_change_found)
    {
        if (const auto [token, n_read] = try_match<tokens::date_version_change>(_sw); token)
        {
            revisions.emplace_back(std::get<tokens::date_version_change>(*token).matched);
            _sw = _sw.substr(n_read);
            return states::in_date_version_change{};
        }
        return states::end{};
    }

    revisions::state_t operator()(states::version_date_author_change_found)
    {
        if (const auto [token, n_read] = try_match<tokens::version_date_author_changes>(_sw); token)
        {
            revisions.emplace_back(std::get<tokens::version_date_author_changes>(*token).matched);
            _sw = _sw.substr(n_read);
            return states::in_version_date_author_change{};
        }
        return states::end{};
    }

    revisions::state_t operator()(states::in_version_description)
    {
        if (const auto [token, n_read] = try_match<tokens::version_description>(_sw); token)
        {
            revisions.emplace_back(std::get<tokens::version_description>(*token).matched);
            _sw = _sw.substr(n_read);
            return states::in_version_description{};
        }
        return states::end{};
    }

    revisions::state_t operator()(states::in_revision_date_description)
    {
        if (const auto [token, n_read] = try_match<tokens::revision_date_description>(_sw); token)
        {
            revisions.emplace_back(std::get<tokens::revision_date_description>(*token).matched);
            _sw = _sw.substr(n_read);
            return states::in_revision_date_description{};
        }
        return states::end{};
    }

    revisions::state_t operator()(states::in_date_version_change)
    {
        if (const auto [token, n_read] = try_match<tokens::date_version_change>(_sw); token)
        {
            revisions.emplace_back(std::get<tokens::date_version_change>(*token).matched);
            _sw = _sw.substr(n_read);
            return states::in_date_version_change{};
        }
        return states::end{};
    }

    revisions::state_t operator()(states::in_version_date_author_change)
    {
        if (const auto [token, n_read] = try_match<tokens::version_date_author_changes>(_sw); token)
        {
            revisions.emplace_back(std::get<tokens::version_date_author_changes>(*token).matched);
            _sw = _sw.substr(n_read);
            return states::in_version_date_author_change{};
        }
        return states::end{};
    }

    using base_transition::operator();
};
} // namespace revisions::transitions

common::revision_t parse_revisions(std::string_view file) noexcept {}
} // namespace sparse::parsers
