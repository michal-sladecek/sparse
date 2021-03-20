set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(cxx_warnings
    "-Wall -Wextra -Wpedantic -Wdouble-promotion -Wnull-dereference -Wimplicit-fallthrough \
    -Wswitch-default -Wuninitialized -Wsuggest-attribute=const -Wsuggest-attribute=noreturn \
    -Wfloat-equal -Wshadow -Wconversion"
)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2 -fstack-clash-protection -fstack-protector-strong ${cxx_warnings}")
