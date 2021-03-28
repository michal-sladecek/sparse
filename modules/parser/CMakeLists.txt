set(target parser)

set(sources
        main.cpp
        )

add_executable(${target}
        ${sources}
        )

target_include_directories(${target}
        PRIVATE
        ${CMAKE_BINARY_DIR}/include
        ${CMAKE_SOURCE_DIR}/include
        )

target_link_libraries(${target}
        PRIVATE
        sparse
        nlohmann_json::nlohmann_json
        )

