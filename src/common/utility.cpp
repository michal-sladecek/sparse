#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <sparse/common/utility.hpp>
#include <string>

namespace sparse::common
{
std::string load_file_into_string(const std::filesystem::path& filename)
{
    std::ifstream file_stream(filename, std::ios::binary);
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