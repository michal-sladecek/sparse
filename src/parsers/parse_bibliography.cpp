#include <iostream>
#include <optional>
#include <regex>
#include <sparse/common/types.hpp>
#include <sparse/parsers/parsers.hpp>
#include <string>


using sparse::common::bibliography_t;


/*
 * Problematic files to parse:
 *     1110V3b_pdf.txt
 *          - the bibliography entries do not contain [] notation
 */

namespace
{
const std::regex bibliography_beginning("(bibliography|literature|reference)", std::regex_constants::icase);
const std::regex bibliography_entry_beginning("^\\s*\\[.*");
const std::regex empty_line("^\\s*$");
const std::regex join_lines_ending_with_dash("-\\s*\n\\s*");
const std::regex join_lines("\\s*\n\\s*");
const std::regex remove_long_whitespaces("\\s\\s*");
const std::regex key_and_entry_split("^\\s*(\\[[^\\s]*\\])\\s*(.*)");

std::vector<std::size_t> find_possible_bibliography_beginnings(const std::string& s) noexcept
{
    std::vector<std::size_t> possible_bibliography_beginnings;

    for (auto iter = std::sregex_iterator(s.begin(), s.end(), bibliography_beginning); iter != std::sregex_iterator(); ++iter)
    {
        possible_bibliography_beginnings.push_back(static_cast<std::size_t>(iter->position(0)));
    }
    return possible_bibliography_beginnings;
}

bool is_line_empty(const std::string& line) noexcept
{
    return std::regex_match(line, empty_line);
}

bool does_line_resemble_bibliography_entry_beginning(const std::string& line) noexcept
{
    return std::regex_match(line, bibliography_entry_beginning);
}

std::string clean_lines(std::string line) noexcept
{
    line = std::regex_replace(line, join_lines_ending_with_dash, "-");
    line = std::regex_replace(line, join_lines, " ");
    line = std::regex_replace(line, remove_long_whitespaces, " ");

    return line;
}

void process_bibliography_entry(sparse::common::bibliography_t& bibliography, const std::string& line) noexcept
{
    const auto joined_line = clean_lines(line);

    std::smatch split_matches;


    std::regex_match(joined_line, split_matches, key_and_entry_split);

    if (split_matches.size() < 3)
    {
        return;
    }

    bibliography[split_matches[1]] = split_matches[2];
}


sparse::common::bibliography_t get_bibliography_beginning_at(const std::string& s, std::size_t beg) noexcept
{
    constexpr int max_length_of_bibliography_entry = 50;

    std::size_t current_position;
    std::size_t previous_position = beg;
    sparse::common::bibliography_t bibliography;

    std::string current_bibliography_entry;

    bool currently_parsing_bibliography_entry = false;


    for (auto lines_since_last_bibliography_entry = 0; lines_since_last_bibliography_entry < max_length_of_bibliography_entry &&
                                                       (current_position = s.find('\n', previous_position + 1)) != std::string::npos;
         ++lines_since_last_bibliography_entry)
    {
        const auto line = s.substr(previous_position, current_position - previous_position);

        if (does_line_resemble_bibliography_entry_beginning(line))
        {
            process_bibliography_entry(bibliography, current_bibliography_entry);

            // Finding new bibliography entry resets the counter.
            lines_since_last_bibliography_entry  = 0;
            current_bibliography_entry           = line;
            currently_parsing_bibliography_entry = true;
        }
        else if (is_line_empty(line) && currently_parsing_bibliography_entry)
        {
            // Finding empty line usually means the end of bibliography entry
            process_bibliography_entry(bibliography, current_bibliography_entry);

            currently_parsing_bibliography_entry = false;
            current_bibliography_entry           = "";
        }
        else if (currently_parsing_bibliography_entry)
        {
            current_bibliography_entry += line;
        }

        previous_position = current_position;
    }

    return bibliography;
}

} // namespace


namespace sparse::parsers
{
std::optional<bibliography_t> parse_bibliography(const std::string& whole_file) noexcept
{
    const auto possible_bibliography_beginnings = find_possible_bibliography_beginnings(whole_file);

    bibliography_t biggest_bibliography;


    for (const auto i : possible_bibliography_beginnings)
    {
        /*
         * Usually there are few false positives - for example if text bibliography is in table of contents
         * In the case there is more of them, we try to parse each one
         * And take the one that contains most entries, as it is the one that is most likely to not be a false positive
         */
        const auto possible_bibliography = get_bibliography_beginning_at(whole_file, i);
        if (possible_bibliography.size() > biggest_bibliography.size())
        {
            biggest_bibliography = possible_bibliography;
        }
    }

    if (biggest_bibliography.empty())
    {
        return {};
    }

    return biggest_bibliography;
}
} // namespace sparse::parsers
