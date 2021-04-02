#pragma once


#include <filesystem>
#include <string>
#include <unordered_map>

namespace sparse::common
{
namespace detail
{
std::string ltrim(std::string str, const std::string& chars = "\t\n\v\f\r ");
std::string rtrim(std::string str, const std::string& chars = "\t\n\v\f\r ");
} // namespace detail

std::string trim(std::string str, const std::string& chars = "\t\n\v\f\r ");

std::string load_file_into_string(const std::filesystem::path& filename);
} // namespace sparse::common
