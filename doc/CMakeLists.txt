set(target doc)

if (NOT BUILD_DOC)
    return()
endif ()

find_package(Doxygen)
if (NOT DOXYGEN_FOUND)
    message("Doxygen missing")
    return()
endif ()

set(doxyfile_src "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")
set(doxyfile_bin "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

configure_file(${doxyfile_src} ${doxyfile_bin} COPYONLY)

add_custom_target(${target}
        ALL
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${DOXYGEN} ${doxyfile_bin}
        COMMENT "Generating documentation"
        )
