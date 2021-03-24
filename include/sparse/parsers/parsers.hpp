#pragma once
#include <optional>
#include <sparse/common/types.hpp>
#include <string>


namespace sparse::parsers
{
std::optional<sparse::common::bibliography_t> parse_bibliography(const std::string&) noexcept;
}
