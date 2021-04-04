#pragma once
#include <optional>
#include <sparse/common/types.hpp>
#include <string>


namespace sparse::parsers
{
/**
 * @brief Parses bibliography
 * @param the whole loaded file
 * @return parsed bibliography
 */
std::optional<common::bibliography_t> parse_bibliography(const std::string& whole_file) noexcept;

/**
 * @brief Parses title
 * Parsing is done using several patterns, starting from the most common one
 * @param file stringview of the whole file
 * @return string with title if successful, std::nullopt otherwise
 */
std::optional<common::title_t> parse_title(std::string_view file);

/**
 * @brief parses versions of the certificate
 * @param certificate the whole loaded certificate
 * @return parsed versions
 */
common::versions_t parse_versions(const std::string& certificate) noexcept;

std::optional<common::table_of_contents_t> parse_toc(const std::string& whole_file) noexcept;
} // namespace sparse::parsers
