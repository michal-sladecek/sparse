#include <filesystem>
#include <fstream>
#include <iostream>
#include <sparse/common/utility.hpp>
#include <string>


namespace sparse::common
{
std::string load_file_into_string(const std::filesystem::path& filename)
{
    constexpr unsigned int MB            = 1024 * 1024;
    constexpr unsigned int max_file_size = 30 * MB;

    if (!std::filesystem::exists(filename))
    {
        throw std::runtime_error("File does not exist: " + filename.string());
    }


    if (std::filesystem::file_size(filename) > max_file_size)
    {
        throw std::runtime_error("File too large: " + filename.string());
    }
    std::ifstream file_stream(filename, std::ios::binary);
    if (!file_stream.is_open())
    {
        throw std::runtime_error("Error when opening file " + filename.string());
    }
    return std::string(std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>());
}

std::string_view trim_line(std::string_view sw_)
{
    const auto start = std::distance(sw_.begin(), std::find_if(sw_.begin(), sw_.end(), [](auto c) { return !std::isspace(c); }));
    const auto end   = std::distance(sw_.rbegin(), std::find_if(sw_.rbegin(), sw_.rend(), [](auto c) { return !std::isspace(c); }));
    return sw_.substr(start, sw_.size() - (end + start));
}

std::string trim_line(std::string sw_)
{
    const auto start = std::distance(sw_.begin(), std::find_if(sw_.begin(), sw_.end(), [](auto c) { return !std::isspace(c); }));
    const auto end   = std::distance(sw_.rbegin(), std::find_if(sw_.rbegin(), sw_.rend(), [](auto c) { return !std::isspace(c); }));
    return sw_.substr(start, sw_.size() - (end + start));
}

void to_json(nlohmann::json& j, const versions_t& versions)
{
    const auto add_not_empty = [&j](const std::string& key, const std::vector<std::string>& value) {
        if (!value.empty())
        {
            j[key] = value;
        }
    };

    add_not_empty("eal", versions.eal);
    add_not_empty("global_platform", versions.global_platform);
    add_not_empty("java_card", versions.java_card);
    add_not_empty("sha", versions.sha);
    add_not_empty("rsa", versions.rsa);
    add_not_empty("ecc", versions.ecc);
    add_not_empty("des", versions.des);
}


void to_json(nlohmann::json& j, const revision_t& revision)
{
    j["version"]     = revision.version;
    j["date"]        = revision.date;
    j["description"] = revision.description;
    if (!revision.author.empty())
        j["author"] = revision.author;
}

void to_json(nlohmann::json& j, const section_t& section)
{
    nlohmann::json array = nlohmann::json::array();
    array.push_back(section.id);
    array.push_back(section.name);
    array.push_back(section.page_number);
    j = array;
}

} // namespace sparse::common
