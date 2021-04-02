#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sparse/common/utility.hpp>
#include <string>


namespace sparse::common
{
namespace detail
{
std::string ltrim(std::string str, const std::string& chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string rtrim(std::string str, const std::string& chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

} // namespace detail

// implementation borrowed from http://www.martinbroadhurst.com/how-to-trim-a-stdstring.html
std::string trim(std::string str, const std::string& chars)
{
    return detail::ltrim(detail::rtrim(str, chars), chars);
}

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

} // namespace sparse::common
