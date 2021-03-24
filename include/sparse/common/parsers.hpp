#pragma once
#include <optional>
#include <sparse/common/types.hpp>
#include <string>


namespace sparse::common
{
std::optional<bibliography_t> parse_bibliography(const std::string&) noexcept;
}
