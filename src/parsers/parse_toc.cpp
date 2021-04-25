//
// Created by a on 4/2/2021.
//
#include <algorithm>
#include <optional>
#include <regex>
#include <sparse/common/types.hpp>
#include <sparse/parsers/parsers.hpp>
#include <string>

namespace
{
const std::regex toc_beginning(R"(.*(content|INDEX).*)", std::regex_constants::icase);
const std::regex toc_split(R"(^\s*([0-9A-Z\.]+)\s+(.+[^\.\s])[\.\s]+([0-9]+)\s*$)");
const std::regex toc_double_split(R"(^\s*([0-9A-Z\.]+)\s+(.+[^\.\s])[\.\s]+([0-9]+)\s+([0-9A-Z\.]+)\s+(.+[^\.\s])[\.\s]+([0-9]+)\s*$)");


int stoi_fallback(const std::string& s)
{
    // The default value is zero
    int retval = 0;
    try
    {
        retval = std::stoi(s);
    }
    catch (std::invalid_argument invalidArgument)
    {}
    catch (std::out_of_range outOfRange)
    {}

    return retval;
}


std::vector<std::size_t> find_possible_toc_beginnings(const std::string& s) noexcept
{
    std::vector<std::size_t> possible_toc_beginnings;

    for (auto iter = std::sregex_iterator(s.begin(), s.end(), toc_beginning); iter != std::sregex_iterator(); ++iter)
    {
        possible_toc_beginnings.push_back(static_cast<std::size_t>(iter->position(0)));
    }
    return possible_toc_beginnings;
}


bool toc_sort(sparse::common::section_t const& lhs, sparse::common::section_t const& rhs)
{
    if (lhs.page_number != rhs.page_number)
    {
        return lhs.page_number < rhs.page_number;
    }
    return lhs.id < rhs.id;
}

sparse::common::table_of_contents_t get_toc(const std::string& s, std::size_t beg) noexcept
{
    constexpr int max_length_of_toc           = 10;
    constexpr int num_double_line_match       = 7;
    constexpr int num_single_line_match       = 4;
    constexpr int minimal_section_name_length = 3;

    std::size_t current_position;
    std::size_t previous_position = beg;
    sparse::common::table_of_contents_t table_of_contents;


    for (auto lines_since_last_toc_entry = 0;
         lines_since_last_toc_entry < max_length_of_toc && (current_position = s.find('\n', previous_position + 1)) != std::string::npos;
         ++lines_since_last_toc_entry)
    {
        const auto line = s.substr(previous_position, current_position - previous_position);

        std::smatch split_matches_double_line, split_matches_single_line;
        std::regex_match(line, split_matches_double_line, toc_double_split);
        std::regex_match(line, split_matches_single_line, toc_split);


        if (split_matches_double_line.size() >= num_double_line_match && split_matches_double_line[2].length() > minimal_section_name_length)
        {
            sparse::common::section_t section_l, section_r;
            section_l.id = split_matches_double_line[1];
            if (section_l.id.ends_with('.'))
            {
                section_l.id.pop_back();
            }
            section_l.name        = split_matches_double_line[2];
            section_l.page_number = stoi_fallback(split_matches_double_line[3]);
            table_of_contents.push_back(section_l);

            section_r.id = split_matches_double_line[4];
            if (section_r.id.ends_with('.'))
            {
                section_r.id.pop_back();
            }
            section_r.name        = split_matches_double_line[5];
            section_r.page_number = stoi_fallback(split_matches_double_line[6]);
            table_of_contents.push_back(section_r);
            lines_since_last_toc_entry = 0;
        }

        else if (split_matches_single_line.size() >= num_single_line_match && split_matches_single_line[2].length() > minimal_section_name_length)
        {
            sparse::common::section_t section;
            section.id = split_matches_single_line[1];
            if (section.id.ends_with('.'))
            {
                section.id.pop_back();
            }
            section.name        = split_matches_single_line[2];
            section.page_number = stoi_fallback(split_matches_single_line[3]);
            table_of_contents.push_back(section);
            lines_since_last_toc_entry = 0;
        }

        previous_position = current_position;
    }

    std::sort(table_of_contents.begin(), table_of_contents.end(), &toc_sort);

    return table_of_contents;
}


} // namespace


namespace sparse::parsers
{
std::optional<common::table_of_contents_t> parse_toc(const std::string& whole_file) noexcept
{
    const auto possible_toc = find_possible_toc_beginnings(whole_file);

    std::optional<common::table_of_contents_t> biggest_toc;


    for (const auto i : possible_toc)
    {
        /*
         * Usually there are few false positives - for example if text bibliography is in table of contents
         * In the case there is more of them, we try to parse each one
         * And take the one that contains most entries, as it is the one that is most likely to not be a false positive
         */
        const auto possible_toc = get_toc(whole_file, i);
        if (!biggest_toc.has_value())
        {
            biggest_toc = possible_toc;
        }
        if (biggest_toc.has_value() && possible_toc.size() > biggest_toc.value().size())
        {
            biggest_toc = possible_toc;
        }
    }

    return biggest_toc;
}
} // namespace sparse::parsers