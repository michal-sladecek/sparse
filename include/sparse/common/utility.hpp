#pragma once


#include <filesystem>
#include <nlohmann/json.hpp>
#include <sparse/common/types.hpp>
#include <string>
#include <unordered_map>

namespace sparse::common
{
std::string load_file_into_string(const std::filesystem::path& filename);

/**
 * @brief to_json
 * @param j json
 * @param versions versions object
 */

void to_json(nlohmann::json& j, const versions_t& versions);
} // namespace sparse::common
