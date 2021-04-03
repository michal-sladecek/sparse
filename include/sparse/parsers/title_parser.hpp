#pragma once

#include <optional>
#include <sparse/common/types.hpp>
#include <string_view>

namespace sparse::parsers
{
/**
 * @brief Parses bibliography title
 * @param file_ stringview of the whole file
 * @return string with title if successful, std::nullopt otherwise
 */
std::optional<common::title_t> parse_title(std::string_view file_);

} // namespace sparse::parsers
