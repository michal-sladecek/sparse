set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # using GCC
    set(additional_warnings "-Wsuggest-attribute=const -Wsuggest-attribute=noreturn")
    set(stack_clash_protection "-fstack-clash-protection")
endif()

set(cxx_warnings
    "-Wall -Wextra -Wpedantic -Wdouble-promotion -Wnull-dereference -Wimplicit-fallthrough \
    -Wswitch-default -Wuninitialized -Wfloat-equal -Wshadow -Wconversion ${additional_warnings}"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2 ${stack_clash_protection} -fstack-protector-strong ${cxx_warnings}")
