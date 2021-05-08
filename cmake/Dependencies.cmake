Include(FetchContent)

# Add Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.4
)

FetchContent_MakeAvailable(Catch2)

# Add nlohmann json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
    GIT_TAG v3.9.1
)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Add Compile-time regular expressions
FetchContent_Declare(
    ctre
    GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
    GIT_TAG v3.3.4
)

FetchContent_GetProperties(ctre)
if(NOT ctre_POPULATED)
  FetchContent_Populate(ctre)
  add_subdirectory(${ctre_SOURCE_DIR} ${ctre_BINARY_DIR})
endif()
