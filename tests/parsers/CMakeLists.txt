set(target tests_parsers)

set(sources
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_parse_bibliography.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_parse_versions.cpp
        )


file(GLOB test_data "test_data/*.txt")
file(COPY ${test_data} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


add_executable(${target} ${sources})
add_executable(${PROJECT_NAME}::tests::${target} ALIAS ${target})

target_include_directories(${target}
        PRIVATE
        ${CMAKE_BINARY_DIR}/include
        ${CMAKE_SOURCE_DIR}/include
        )

target_link_libraries(${target}
        PRIVATE
        Catch2::Catch2
        sparse::sparse
        # nlohmann_json::nlohmann_json
        )

add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)
