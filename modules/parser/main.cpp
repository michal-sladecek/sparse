#include <iostream>
#include <nlohmann/json.hpp>
#include <sparse/common/types.hpp>
#include <sparse/common/utility.hpp>
#include <sparse/parsers/parsers.hpp>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

struct options_t
{
    bool parse_title{};
    bool parse_toc{};
    bool parse_versions{};
    bool parse_revisions{};
    bool parse_bib{};

    bool use_cout{};
    std::vector<fs::path> items_to_parse;
};

void print_help_message()
{
    std::cout << "SPARSE TODO: Description\n";
    std::cout << "\nSupported program arguments:\n";
}

options_t parse_program_options(int argc_, char* argv_[])
{
    options_t o{};
    bool some_set{};
    bool print_help{};
    const auto arg_map = [&]() {
        std::map<std::string_view, std::pair<std::function<void()>, std::string_view>> m;
        // clang-format off
        m.insert({"--title",        {[&]() { some_set = o.parse_title = true; },     "Parse document Title."}});
        m.insert({"--toc",          {[&]() { some_set = o.parse_toc = true; },       "Parse Table of Content."}});
        m.insert({"--versions",     {[&]() { some_set = o.parse_versions = true; },  "Parse Versions."}});
        m.insert({"--revisions",    {[&]() { some_set = o.parse_revisions = true; }, "Parse Revisions."}});
        m.insert({"--bibliography", {[&]() { some_set = o.parse_bib = true; },       "Parse Bibliography."}});
        m.insert({"--out",          {[&]() { o.use_cout = true; },                   "Print result json to standard output."}});
        m.insert({"--help",         {[&]() { print_help = true; },                   "Show program options and exit."}});
        m.insert({"-h",             {[&]() { print_help = true; },                   ""}});
        // clang-format on
        return m;
    }();

    for (int i = 1; i < argc_; ++i)
    {
        if (const std::string_view arg{argv_[i]}; arg_map.contains(arg))
        {
            std::invoke(arg_map.at(arg).first);
        }
        else
        {
            o.items_to_parse.emplace_back(arg);
        }
    }

    if (print_help || argc_ <= 1)
    {
        print_help_message();
        for (const auto& [key, val] : arg_map)
        {
            if (std::string_view msg{val.second}; !msg.empty())
            {
                std::cout << "  " << std::setw(18) << std::left << key << msg << std::endl;
            }
        }
        exit(EXIT_SUCCESS);
    }
    if (!some_set)
    {
        o.parse_bib = o.parse_toc = o.parse_title = o.parse_versions = o.parse_revisions = true;
    }
    for (const auto& path : o.items_to_parse)
    {
        if (!fs::is_directory(path) && !fs::is_regular_file(path))
        {
            std::cerr << "Error: Parameter " << path << " is neither file nor directory.";
            exit(EXIT_FAILURE);
        }
    }

    return o;
}


int main(int argc, char* argv[])
{
    const auto options    = parse_program_options(argc, argv);
    const auto whole_file = sparse::common::load_file_into_string(argv[1]);

    nlohmann::json output;
    const auto versions     = sparse::parsers::parse_versions(whole_file);
    const auto bibliography = sparse::parsers::parse_bibliography(whole_file);

    if (const auto title = sparse::parsers::parse_title(whole_file))
    {
        output["title"] = *title;
    }

    output["versions"] = versions;
    if (bibliography.has_value())
    {
        output["bibliography"] = bibliography.value();
    }

    std::cout << std::setw(4) << output << std::endl;
    return EXIT_SUCCESS;
}
