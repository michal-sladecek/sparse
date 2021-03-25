#pragma once


#include <string>
#include <unordered_map>
#include <filesystem>

namespace sparse::common
{
void print_program_args(int argc_, char* argv_[]);
std::string load_file_into_string(const std::filesystem::path& filename);

} // namespace sparse::common
