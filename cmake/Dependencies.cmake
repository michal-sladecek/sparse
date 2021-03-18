set(ext_dir ${CMAKE_SOURCE_DIR}/external)

add_subdirectory("${ext_dir}/Catch2")

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory("${ext_dir}/nlohmann_json")
