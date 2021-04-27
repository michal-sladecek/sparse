#include <iostream>
#include <regex>
#include <sparse/common/utility.hpp>
#include <sparse/parsers/parsers.hpp>
#include <unordered_map>

namespace sparse::parsers
{
namespace detail
{
std::string repetitions_of(const std::string s)
{
    return "\n{0,2}(" + s + ")+\n{1,2}";
}


namespace tokens
{
static const std::string start     = "( |\\t)*";
static const std::string delimeter = "( |\\t)+";
static const std::string months    = "(January|February|March|April|May|June|July|August|September|October|November|December)";

static const std::string header                           = "^(.|\\n|\\n\\r)*(Revision History|Version Control)";
static const std::string version_description_header       = "^(.|\\n|\\n\\r)*Version[ \\t]+Description of change";
static const std::string revision_date_description_header = "^(.|\\n|\\n\\r)*(Rev|Revision|Version)"
                                                            "[ \\t]+(Date|Release Date)[ \\t]+(Description number|Description)";
static const std::string date_version_description_header        = "^(.|\\n|\\n\\r)*Date[ \\t]+Version[ \\t]+Change notice";
static const std::string version_date_author_description_header = "^(.|\\n|\\n\\r)*Version[ \\t]+Date[ \\t]+Author[ \\t]+Changes to Previous version";

static const std::string version     = "(v|Rev\\. |Version )?\\d\\.\\d";
static const std::string date        = "(\\d{4}\\-\\d{1,2}\\-\\d{1,2}|\\d{1,2}.\\d{1,2}.\\d{4}|\\d{1,2}[ \\-]" + months + "[ \\-]\\d{4})";
static const std::string author      = "[a-zA-Z]+ [a-zA-Z]+ ([a-zA-Z]+)?";
static const std::string description = ".*(\\r\\n|\\n)";
} // namespace tokens

const static std::regex header(tokens::header);

const static std::regex version(tokens::start + tokens::version);
const static std::regex start_version("\n" + tokens::version);

const static std::regex date(tokens::start + tokens::date);
const static std::regex start_date("\n" + tokens::version);

const static std::regex author(tokens::start + tokens::author);
const static std::regex description(tokens::start + tokens::description);


const static std::regex version_description_header(tokens::version_description_header);
const static std::regex revision_date_description_header(tokens::revision_date_description_header);
const static std::regex date_version_description_header(tokens::date_version_description_header);
const static std::regex version_date_author_description_header(tokens::version_date_author_description_header);

const static std::regex version_description_item(repetitions_of(tokens::version + tokens::delimeter + tokens::description));
const static std::regex version_date_description_item(repetitions_of(tokens::version + tokens::delimeter + tokens::date + tokens::delimeter +
                                                                     tokens::description));
const static std::regex date_version_description_item(repetitions_of(tokens::date + tokens::delimeter + tokens::version + tokens::delimeter +
                                                                     tokens::description));
const static std::regex version_date_author_description_item(repetitions_of(tokens::version + tokens::delimeter + tokens::date + tokens::delimeter +
                                                                            tokens::author + tokens::delimeter + tokens::description));
namespace revision_type
{
using type                                        = int;
const static type version_description             = 0;
const static type version_date_description        = 1;
const static type date_version_description        = 2;
const static type version_date_author_description = 3;
} // namespace revision_type


// clang-format off
const static std::vector<std::tuple<std::regex, revision_type::type>> header_type {
    {version_description_header, revision_type::version_description},
    {revision_date_description_header, revision_type::version_date_description},
    {date_version_description_header, revision_type::date_version_description},
    {version_date_author_description_header, revision_type::version_date_author_description},
};

const static std::unordered_map<revision_type::type, std::regex> type_item {
    {revision_type::version_description, version_description_item},
    {revision_type::version_date_description, version_date_description_item},
    {revision_type::date_version_description, date_version_description_item},
    {revision_type::version_date_author_description, version_date_author_description_item},
};

const static std::unordered_map<revision_type::type, std::regex> type_first {
    {revision_type::version_description, start_version},
    {revision_type::version_date_description, start_version},
    {revision_type::date_version_description, start_date},
    {revision_type::version_date_author_description, start_version},
};
// clang-format on
} // namespace detail


std::vector<std::pair<std::string, std::string>> get_items(std::string s, detail::revision_type::type type)
{
    const auto it = detail::type_first.find(type);
    if (it == detail::type_first.end())
    {
        return {};
    }
    const auto re = it->second;

    std::vector<std::string> revisions;
    std::vector<std::string> rest;
    std::smatch sm;

    while (std::regex_search(s, sm, re))
    {
        revisions.push_back(sm.str());

        const auto split_on = static_cast<std::size_t>(sm.position());
        const auto r        = s.substr(0, split_on);
        if (!r.empty())
        {
            rest.push_back(r);
        }

        s = s.substr(split_on + static_cast<std::size_t>(sm.length()));
    }
    rest.push_back(s);

    std::vector<std::pair<std::string, std::string>> ret;
    for (auto i = 0ul; i < std::max(revisions.size(), rest.size()); ++i)
    {
        std::string r = (i < revisions.size()) ? std::move(revisions[i]) : "";
        std::string o = (i < rest.size()) ? std::move(rest[i]) : "";
        ret.emplace_back(std::move(r), std::move(o));
    }
    return ret;
}

std::size_t find_header(const std::string& s)
{
    auto words = std::regex_iterator(s.begin(), s.end(), detail::header);
    for (auto iter = words; iter != std::sregex_iterator(); ++iter)
    {
        return iter->str().size();
    }
    return 0;
}

std::tuple<std::size_t, std::optional<detail::revision_type::type>> find_revisions_version(const std::string& s)
{
    for (const auto& [header_parser, item_parser] : detail::header_type)
    {
        auto words = std::regex_iterator(s.begin(), s.end(), header_parser);
        for (auto iter = words; iter != std::sregex_iterator(); ++iter)
        {
            return {iter->str().size(), {item_parser}};
        }
    }
    return {0, {}};
}

std::string match_str(std::string& s, const std::regex& re)
{
    std::string ret;
    std::smatch sm;
    if (std::regex_search(s, sm, re))
    {
        ret = sm.str();
        s   = s.substr(ret.size(), s.size());
    }
    return common::trim_line(ret);
}


common::revision_t parse_version_description(std::string f, std::string s)
{
    common::revision_t revision;
    revision.version     = match_str(f, detail::version);
    revision.description = common::trim_line(s);
    return revision;
}

common::revision_t parse_version_date_description(std::string f, std::string s)
{
    common::revision_t revision;
    revision.version     = match_str(f, detail::version);
    revision.date        = match_str(s, detail::date);
    revision.description = common::trim_line(s);
    return revision;
}

common::revision_t parse_date_version_description(std::string f, std::string s)
{
    common::revision_t revision;
    revision.version     = match_str(f, detail::version);
    revision.date        = match_str(s, detail::date);
    revision.description = common::trim_line(s);
    return revision;
}

common::revision_t parse_version_date_author_description(std::string f, std::string s)
{
    common::revision_t revision;
    revision.version     = match_str(f, detail::version);
    revision.date        = match_str(s, detail::date);
    revision.author      = match_str(s, detail::author);
    revision.description = common::trim_line(s);
    return revision;
}

std::optional<common::revision_t> parse_r(const std::string f, const std::string& s, detail::revision_type::type t)
{
    using namespace detail;
    switch (t)
    {
    case revision_type::version_description:
        return parse_version_description(f, s);
    case revision_type::version_date_description:
        return parse_version_date_description(f, s);
    case revision_type::date_version_description:
        return parse_date_version_description(f, s);
    case revision_type::version_date_author_description:
        return parse_version_date_author_description(f, s);
    }
    return {};
}

common::revisions_t parse_items(const std::vector<std::pair<std::string, std::string>>& items, detail::revision_type::type t)
{
    common::revisions_t ret;
    for (const auto& [f, s] : items)
    {
        const auto res = parse_r(f, s, t);
        if (res)
        {
            ret.push_back(*res);
        }
    }

    return ret;
}

std::optional<common::revisions_t> parse_revisions(const std::string& file) noexcept
{
    const auto header_end             = find_header(file);
    const std::string& without_header = file.substr(header_end, file.size());

    const auto& [tokens_from, type] = find_revisions_version(without_header);
    if (tokens_from == 0)
    {
        return {};
    }

    const auto& items_str = without_header.substr(tokens_from, without_header.size());

    const auto items = get_items(items_str, *type);
    return parse_items(items, *type);
}
} // namespace sparse::parsers
