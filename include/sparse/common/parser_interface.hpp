#pragma once

#include <string>
#include <optional>

namespace sparse::common
{
    template<typename document_part>
    class parser_interface {
    public:
        virtual std::optional<document_part> parse(const std::string &);
    };
}


