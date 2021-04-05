#pragma once
#include <optional>
#include <sparse/common/types.hpp>
#include <string>


namespace sparse::parsers
{
std::optional<common::bibliography_t> parse_bibliography(const std::string&) noexcept;

/**
 * @brief Parses bibliography title
 * Parsing is done using several patterns, starting from the most common one
 * @param file_ stringview of the whole file
 * @return string with title if successful, std::nullopt otherwise
 */
std::optional<common::title_t> parse_title(std::string_view file);

} // namespace sparse::parsers
