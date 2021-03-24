set(target tests_parsers)

set(sources
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_parse_bibliography.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )

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
        )

add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)
