set(target common)

set(source_path ${CMAKE_CURRENT_SOURCE_DIR})
set(include_path "${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/${target}")

set(sources
    ${source_path}/utility.cpp
)

set(headers
    ${include_path}/utility.hpp
    ${include_path}/types.hpp
    ${include_path}/parser_interface.hpp
)

add_library(${target} OBJECT ${sources} ${headers})
add_library (${PROJECT_NAME}::${target} ALIAS ${target})

target_include_directories(${target}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
)

target_link_libraries(${target}
    PUBLIC
    nlohmann_json::nlohmann_json
)
