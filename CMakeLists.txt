cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

project(sparse 
    VERSION 0.0.1 
    DESCRIPTION  "PA193 Semestral project 2021"
    HOMEPAGE_URL "https://github.com/misoxxx/sparse"
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Project options
option(BUILD_MODULES "Build project modules"  ON)
option(BUILD_DOC     "Generace documentation" ON)


include(Dependencies)
include(CompileOptions)
include(CTest)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(modules)
add_subdirectory(doc)

# Unused variables
if(QT_QMAKE_EXECUTABLE)
endif()
if(CMAKE_C_COMPILER)
endif()
