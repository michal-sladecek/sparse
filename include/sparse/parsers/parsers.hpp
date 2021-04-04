#pragma once
#include <optional>
#include <sparse/common/types.hpp>
#include <string>


namespace sparse::parsers
{
std::optional<common::bibliography_t> parse_bibliography(const std::string&) noexcept;

/**
 * @brief parses versions of the certificate
 * @param certificate the whole loaded certificate
 * @return parsed versions
 */
common::versions_t parse_versions(const std::string& certificate) noexcept;
} // namespace sparse::parsers
