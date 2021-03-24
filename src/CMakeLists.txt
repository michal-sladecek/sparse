set(target sparse)

add_subdirectory(common)

# Create libsparse from object libraries
add_library(${target} $<TARGET_OBJECTS:common>)
add_library(${PROJECT_NAME}::${target} ALIAS ${target})

# TODO install library
