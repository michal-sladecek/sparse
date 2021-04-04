#include <iostream>
#include <nlohmann/json.hpp>
#include <sparse/common/types.hpp>
#include <sparse/common/utility.hpp>
#include <sparse/parsers/parsers.hpp>
#include <sparse/parsers/title_parser.hpp>
#include <string>


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Missing filename argument" << std::endl;
        return EXIT_FAILURE;
    }
    auto whole_file = sparse::common::load_file_into_string(argv[1]);

    nlohmann::json output;

    if (const auto title = sparse::parsers::parse_title(whole_file))
    {
        output["title"] = *title;
    }

    auto bibliography = sparse::parsers::parse_bibliography(whole_file);
    if (bibliography.has_value())
    {
        output["bibliography"] = bibliography.value();
    }

    std::cout << std::setw(4) << output << std::endl;
    return EXIT_SUCCESS;
}
