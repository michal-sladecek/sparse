#include <sparse/common/utility.hpp>

#include <iostream>

namespace sparse::common {

void print_program_args(int argc_, char* argv_[]) {
    for (int i = 1; i < argc_; ++i) {
        std::cout << argv_[i] << std::endl;
    }
}

} // namespace sparse::common
