#include <iostream>
#include <regex>
#include <sparse/parsers/parsers.hpp>
#include <unordered_map>

namespace sparse::parsers
{
namespace detail
{
namespace tokens
{
static const std::string until_end = "\\.*\n"; ///< unitl the end of line
static const std::string header    = "^(.|\\n|\\n\\r)*(Revision History|Version Control)" + until_end;

static const std::string version_description_header       = "^(.|\\n|\\n\\r)*Version[ \\t]+Description of change" + until_end;
static const std::string revision_date_description_header = "^(.|\\n|\\n\\r)*(Rev|Revision|Version)"
                                                            "[ \\t]+(Date|Release Date)[ \\t]+(Description number|Description)" +
                                                            until_end;
static const std::string date_version_description_header = "^(.|\\n|\\n\\r)*Date[ \\t]+Version[ \\t]+Change notice" + until_end;
static const std::string version_date_author_description_header =
    "^(.|\\n|\\n\\r)*Version[ \\t]+Date[ \\t]+Author[ \\t]+Changes to Previous version" + until_end;

static const std::string delimeter = "[ \\t]+";
static const std::string version   = "(v|Rev\\. |Version )?\\d.\\d";
static const std::string date      = "(\\d{4}\\-\\d{1,2}\\-\\d{1,2}|\\d{1,2}.\\d{1,2}.\\d{4}|\\d{1,2})[ \\-]"
                                "(January|February|March|April|May|June|July|August|September|October|November|December)"
                                "[ \\-]\\d{4}";
static const std::string author      = "[a-zA-Z]+ [a-zA-Z]+ ([a-zA-Z]+)?";
static const std::string description = ".*(\\r\\n|\\n)";
} // namespace tokens

const static std::regex header(tokens::header);

const static std::regex version(tokens::version);
const static std::regex date(tokens::date);
const static std::regex author(tokens::author);
const static std::regex description(tokens::description);

const static std::regex version_description_header(tokens::version_description_header);
const static std::regex revision_date_description_header(tokens::revision_date_description_header);
const static std::regex date_version_description_header(tokens::date_version_description_header);
const static std::regex version_date_author_description_header(tokens::version_date_author_description_header);

const static std::regex version_description_item(tokens::version + tokens::delimeter + tokens::description);
const static std::regex version_date_description_item(tokens::version + tokens::delimeter + tokens::date + tokens::delimeter + tokens::description);
const static std::regex date_version_description_item(tokens::date + tokens::delimeter + tokens::version + tokens::delimeter + tokens::description);
const static std::regex version_date_author_description_item(tokens::version + tokens::delimeter + tokens::date + tokens::delimeter + tokens::author +
                                                             tokens::delimeter + tokens::description);

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

const static std::unordered_map<revision_type::type, std::vector<std::regex>> type_parsers {
    {revision_type::version_description, {version, description}},
    {revision_type::version_date_description, {version, date, description}},
    {revision_type::date_version_description, {date, version, description}},
    {revision_type::version_date_author_description, {version, date, author, description}},
};
// clang-format on
} // namespace detail

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

std::vector<std::string> get_items(const std::string& s, detail::revision_type::type t)
{
    std::vector<std::string> items;

    const auto& res = detail::type_item.find(t);
    if (res == detail::type_item.end())
    {
        return {};
    }

    auto words = std::sregex_iterator(s.begin(), s.end(), res->second);
    for (auto iter = words; iter != std::sregex_iterator(); ++iter)
    {
        items.push_back(iter->str());
    }
    return items;
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
    return ret;
}


common::revision_t parse_version_description(std::string s)
{
    common::revision_t revision;
    revision.version     = match_str(s, detail::version);
    revision.description = match_str(s, detail::description);
    return revision;
}

common::revision_t parse_version_date_description(std::string s)
{
    common::revision_t revision;
    revision.version     = match_str(s, detail::version);
    revision.date        = match_str(s, detail::date);
    revision.description = match_str(s, detail::description);
    return revision;
}

common::revision_t parse_date_version_description(std::string s)
{
    common::revision_t revision;
    revision.version     = match_str(s, detail::version);
    revision.date        = match_str(s, detail::date);
    revision.description = match_str(s, detail::description);
    return revision;
}

common::revision_t parse_version_date_author_description(std::string s)
{
    common::revision_t revision;
    revision.version     = match_str(s, detail::version);
    revision.date        = match_str(s, detail::date);
    revision.author      = match_str(s, detail::author);
    revision.description = match_str(s, detail::description);
    return revision;
}


std::optional<common::revision_t> parse_r(const std::string& s, detail::revision_type::type t)
{
    using namespace detail;
    switch (t)
    {
    case revision_type::version_description:
        return parse_version_description(s);
    case revision_type::version_date_description:
        return parse_version_date_description(s);
    case revision_type::date_version_description:
        return parse_date_version_description(s);
    case revision_type::version_date_author_description:
        return parse_version_date_author_description(s);
    }
    return {};
}

common::revisions_t parse_items(const std::vector<std::string>& items, detail::revision_type::type t)
{
    common::revisions_t ret;
    for (const auto& item : items)
    {
        const auto res = parse_r(item, t);
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
    const auto items      = get_items(items_str, *type);
    return parse_items(items, *type);
}
} // namespace sparse::parsers
