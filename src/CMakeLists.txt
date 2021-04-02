set(target sparse)

add_subdirectory(common)
add_subdirectory(parsers)

# Create libsparse from object libraries
add_library(${target} $<TARGET_OBJECTS:common> $<TARGET_OBJECTS:parsers>)


add_library(${PROJECT_NAME}::${target} ALIAS ${target})

target_link_libraries(${target}
        PUBLIC
        nlohmann_json::nlohmann_json
        )

# TODO install library
