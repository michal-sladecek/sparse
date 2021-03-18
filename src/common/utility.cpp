#include <sparse/common/utility.hpp>

#include <iostream>
#include <nlohmann/json.hpp>

namespace sparse::common {

void print_program_args(int argc_, char* argv_[]) {
    using json = nlohmann::json;
    json j = json::array();
    for (int i = 1; i < argc_; ++i) {
        j.push_back(argv_[i]);
    }
    std::cout << j << std::endl;
}

} // namespace sparse::common
