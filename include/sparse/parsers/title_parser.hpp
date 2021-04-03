#pragma once

#include<optional>
#include<string_view>
#include<sparse/common/types.hpp>

namespace sparse::parsers {

std::optional<common::title_t> parse_title(std::string_view file_);

} // namespace sparse::parsers
