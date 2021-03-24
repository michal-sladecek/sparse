#include <iostream>
#include <optional>
#include <regex>
#include <sparse/common/parsers.hpp>
#include <sparse/common/types.hpp>
#include <string>

const int NUMLINESTOENDBIBLIOGRAPHY = 50;


/*
 * Problematic files to parse:
 *     1110V3b_pdf.txt
 *          - the bibliography entries do not contain [] notation
 *     NSCIB-CC-195714_3-STv3.1.txt
 *          - not yet sure why
 *
 *
 */

namespace
{
/*
 * Attempt to find the bibliography by finding keyword "Bibliography"
 *
 */
void find_possible_bibliography_beginnings(const std::string& s, std::vector<int>& possible_bibliography_beginnings)
{
    std::regex bibliography_beginning("(Bibliography|References|Literature|REFERENCE)");
    int length = s.length();
    std::smatch match;

    for (std::sregex_iterator i = std::sregex_iterator(s.begin(), s.end(), bibliography_beginning); i != std::sregex_iterator(); ++i)
    {
        possible_bibliography_beginnings.push_back(i->position(0));
    }
}

bool is_line_empty(const std::string& line)
{
    std::regex empty_line("^\\s*$");
    return std::regex_match(line, empty_line);
}

bool does_line_resemble_bibliography_entry_beginning(const std::string& line)
{
    std::regex bibliography_entry_beginning("^\\s*\\[.*");
    return std::regex_match(line, bibliography_entry_beginning);
}

std::string clean_lines(const std::string& line)
{
    std::string result = line;

    std::regex join_lines_ending_with_dash("-\\s*\n\\s*");
    std::regex join_lines("\\s*\n\\s*");
    std::regex remove_long_whitespaces("\\s\\s*");

    result = std::regex_replace(result, join_lines_ending_with_dash, "-");
    result = std::regex_replace(result, join_lines, " ");
    result = std::regex_replace(result, remove_long_whitespaces, " ");

    return result;
}

void process_bibliography_entry(sparse::common::bibliography_t& bibliography, const std::string& line)
{
    std::string joined_line = clean_lines(line);

    std::smatch split_matches;

    std::regex key_and_entry_split("^\\s*(\\[[^\s]*\\])\\s*(.*)");
    std::regex_match(joined_line, split_matches, key_and_entry_split);

    if (split_matches.size() < 3)
    {
        return;
    }

    bibliography[split_matches[1]] = split_matches[2];
}


sparse::common::bibliography_t get_bibliography_beginning_at(const std::string& s, int beg)
{
    std::string::size_type current_position;
    std::string::size_type previous_position = beg;


    sparse::common::bibliography_t bibliography;


    /*
     * If we are currently parsing a bibliography and we do not find new entry for NUMLINESTOENDBIBLIOGRAPHY, we stop parsing.
     */
    unsigned int lines_until_finished_parsing = NUMLINESTOENDBIBLIOGRAPHY;

    std::string current_bibliography_entry = "";

    bool currently_parsing_bibliography_entry = false;


    while (lines_until_finished_parsing > 0 && (current_position = s.find('\n', previous_position + 1)) != std::string::npos)
    {
        std::string line = s.substr(previous_position, current_position - previous_position);

        bool current_line_contains_beginning_of_bibliography = does_line_resemble_bibliography_entry_beginning(line);
        bool current_line_is_empty                           = is_line_empty(line);

        if (current_line_contains_beginning_of_bibliography)
        {
            process_bibliography_entry(bibliography, current_bibliography_entry);

            // Finding new bibliography entry resets the counter.
            lines_until_finished_parsing         = NUMLINESTOENDBIBLIOGRAPHY;
            current_bibliography_entry           = line;
            currently_parsing_bibliography_entry = true;
        }
        else if (current_line_is_empty && currently_parsing_bibliography_entry)
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


        --lines_until_finished_parsing;
        previous_position = current_position;
    }

    return bibliography;
}

} // namespace


namespace sparse::common
{
std::optional<bibliography_t> parse_bibliography(const std::string& whole_file)
{
    std::vector<int> possible_bibliography_beginnings;


    // We find possible beginnings of bibliography
    find_possible_bibliography_beginnings(whole_file, possible_bibliography_beginnings);


    bibliography_t biggest_bibliography;


    for (auto i : possible_bibliography_beginnings)
    {
        /*
         * Usually there are few false positives - for example if text bibliography is in table of contents
         * In the case there is more of them, we try to parse each one
         * And take the one that contains most entries, as it is the one that is most likely to not be a false positive
         */
        bibliography_t possible_bibliography = get_bibliography_beginning_at(whole_file, i);
        if (possible_bibliography.size() > biggest_bibliography.size())
        {
            biggest_bibliography = possible_bibliography;
        }
    }

    if (biggest_bibliography.size() == 0)
    {
        return std::optional<bibliography_t>();
    }

    return std::optional<bibliography_t>(biggest_bibliography);
}
} // namespace sparse::common