#pragma once
#include <sparse/common/types.hpp>
#include <string>
#include <optional>


namespace sparse::common
{
    std::optional<bibliography_t> parse_bibliography(const std::string &);
}