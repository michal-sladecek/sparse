cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

project(sparse 
    VERSION 0.1.0
    DESCRIPTION  "PA193 Semestral project 2021"
    HOMEPAGE_URL "https://github.com/misoxxx/sparse"
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Project options
option(BUILD_MODULES "Build project modules"  ON)
option(BUILD_DOC     "Generace documentation" ON)

# Set up sanitizers for project
# "address", "memory" and "thread" are mutually exclusive, "leak" requires the "address" sanitizer
set(ECM_ENABLE_SANITIZERS CACHE STRING "Any of: undefined;thread;address;leak")

include(Dependencies)
include(CompileOptions)
include(CTest)
include(ECMEnableSanitizers)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(modules)
add_subdirectory(doc)

# Unused variables
if(QT_QMAKE_EXECUTABLE)
endif()
if(CMAKE_C_COMPILER)
endif()
