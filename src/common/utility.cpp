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

/**
 * @brief to_json
 * @param j json
 * @param versions versions object
 */
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
} // namespace sparse::common
