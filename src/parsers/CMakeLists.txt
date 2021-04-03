set(target parsers)

set(source_path ${CMAKE_CURRENT_SOURCE_DIR})
set(include_path "${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/${target}")

set(sources
    ${source_path}/parse_bibliography.cpp
    ${source_path}/title_parser.cpp
)

set(headers
    ${include_path}/parsers.hpp
    ${include_path}/title_parser.hpp
)

add_library(${target} OBJECT ${sources} ${headers})
add_library(${PROJECT_NAME}::${target} ALIAS ${target})

target_include_directories(${target}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
)

target_link_libraries(${target}
    PUBLIC
    ctre
)
