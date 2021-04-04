#pragma once


#include <filesystem>
#include <string>
#include <unordered_map>

namespace sparse::common
{
std::string load_file_into_string(const std::filesystem::path& filename);

/**
 * @brief Removes whitespace characters from both ends of given string
 * @param sw_ string view
 * @return trimmed string view
 */
std::string_view trim_line(std::string_view sw_);

} // namespace sparse::common
