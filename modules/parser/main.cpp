#include <iostream>
#include <nlohmann/json.hpp>
#include <sparse/common/types.hpp>
#include <sparse/common/utility.hpp>
#include <sparse/parsers/parsers.hpp>
#include <string>


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Missing filename argument" << std::endl;
    }
    auto whole_file = sparse::common::load_file_into_string(argv[1]);

    nlohmann::json output;

    const auto versions     = sparse::parsers::parse_versions(whole_file);
    const auto bibliography = sparse::parsers::parse_bibliography(whole_file);
    if (bibliography.has_value())
    {
        output["bibliography"] = bibliography.value();
    }
    output["versions"] = versions.value();

    std::cout << std::setw(4) << output;
}
