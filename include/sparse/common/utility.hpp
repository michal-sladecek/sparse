#pragma once

#include <string>
#include <unordered_map>


namespace sparse::common
{
void print_program_args(int argc_, char* argv_[]);
std::string load_file_into_string(const std::string& filename);

} // namespace sparse::common