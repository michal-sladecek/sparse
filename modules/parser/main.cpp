#include <fstream>
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

/**
 * @brief Prints program description to standard output
 */
void print_help_message()
{
    std::cout << "SPARSE: Certificate text-to-json parser\n\n";
    std::cout << "Program expects one or more paths either to directory with \n";
    std::cout << "certifiactes, or to certificate itself, program will create \n";
    std::cout << "'.json' file with parsed content in the same directory as \n";
    std::cout << "the input file.\n";
    std::cout << "\nSupported program arguments:\n";
}

/**
 * @brief Parses program options
 * @param argc_ argc
 * @param argv_ argv
 * @return filled options_t struct
 */
options_t parse_program_options(int argc_, char* argv_[]);

/**
 * @brief Parses one input file according to given options
 * Creates output file with '.json' extension
 * in the same directory as intput file
 * @param path_ path to input file
 * @param options_ parsed program options
 */
void parse_file(fs::path path_, const options_t& options_);

int main(int argc, char* argv[])
{
    const auto options = parse_program_options(argc, argv);
    for (const fs::path& item : options.items_to_parse)
    {
        if (fs::is_regular_file(item))
        {
            parse_file(item, options);
            continue;
        }
        for (const auto& p : fs::directory_iterator(item))
        {
            if (p.path().extension() == ".txt")
            {
                parse_file(p, options);
            }
        }
    }
    return EXIT_SUCCESS;
}


options_t parse_program_options(int argc_, char* argv_[])
{
    options_t o{};
    bool some_set{};
    bool print_help{};
    const auto arg_map = [&o, &some_set, &print_help]() {
        std::map<std::string_view, std::pair<std::function<void()>, std::string_view>> m;
        // clang-format off
        m.insert({"--bibliography", {[&]() { some_set = o.parse_bib = true; },       "Parse Bibliography."}});
        m.insert({"--help",         {[&]() { print_help = true; },                   "Show program options and exit."}});
        m.insert({"-h",             {[&]() { print_help = true; },                   ""}});
        m.insert({"--out",          {[&]() { o.use_cout = true; },                   "Print result json to standard output."}});
        m.insert({"--revisions",    {[&]() { some_set = o.parse_revisions = true; }, "Parse Revisions."}});
        m.insert({"--title",        {[&]() { some_set = o.parse_title = true; },     "Parse document Title."}});
        m.insert({"--toc",          {[&]() { some_set = o.parse_toc = true; },       "Parse Table of Content."}});
        m.insert({"--versions",     {[&]() { some_set = o.parse_versions = true; },  "Parse Versions."}});
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
    if (!some_set){
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

void parse_file(fs::path path_, const options_t& options_)
{
    try
    {
        const auto whole_file = sparse::common::load_file_into_string(path_);
        nlohmann::json output;
        std::cerr << "Parsing " << path_ << std::endl;
        if (options_.parse_title)
        {
            if (const auto title = sparse::parsers::parse_title(whole_file))
            {
                output["title"] = *title;
            }
        }
        if (options_.parse_versions)
        {
            const auto versions = sparse::parsers::parse_versions(whole_file);
            output["versions"]  = versions;
        }
        if(options_.parse_toc){
            const auto toc = sparse::parsers::parse_toc(whole_file);
            if (toc.has_value())
            {
                output["table_of_contents"] = toc.value();
            }
        }
        // TODO: add Revisions
        if (options_.parse_bib)
        {
            if (const auto bibliography = sparse::parsers::parse_bibliography(whole_file))
            {
                output["bibliography"] = bibliography.value();
            }
        }



        if (options_.use_cout)
        {
            std::cout << std::setw(4) << output << std::endl;
        }

        path_.replace_extension(".json");
        std::ofstream ofs{path_};
        ofs << std::setw(4) << output << std::endl;
    }
    catch (const std::runtime_error& e_)
    {
        std::cerr << "Failed to parse " << path_ << "\n";
        std::cerr << "Error: " << e_.what() << std::endl;
    }
}
