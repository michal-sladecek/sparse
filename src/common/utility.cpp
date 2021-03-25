#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
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


void print_program_args(int argc_, char* argv_[])
{
    using json = nlohmann::json;
    json j     = json::array();
    for (int i = 1; i < argc_; ++i)
    {
        j.push_back(argv_[i]);
    }
    std::cout << j << std::endl;
}

} // namespace sparse::common