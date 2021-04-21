set(target parsers)

set(source_path ${CMAKE_CURRENT_SOURCE_DIR})
set(include_path "${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/${target}")

set(sources
    ${source_path}/parse_bibliography.cpp
    ${source_path}/title_parser.cpp
    ${source_path}/parse_versions.cpp
    ${source_path}/parse_revisions.cpp
)

set(headers
    ${include_path}/parsers.hpp
    ${include_path}/tokens.hpp
    ${include_path}/state_machine.hpp
    ${include_path}/revisions/state_machine.hpp
    ${include_path}/revisions/tokens.hpp
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
    nlohmann_json::nlohmann_json
)
