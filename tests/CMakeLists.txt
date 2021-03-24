if (NOT BUILD_TESTING)
    return()
endif ()


set(test_data
        1107a_pdf.txt
        Certification_Report_NSCIB-CC-200689-CR.txt
        NSCIB-CC-195714_3-STv3.1.txt
        )

foreach (df ${test_data})
    file(COPY ./test_data/${df} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach ()

add_subdirectory(common)

# TODO add catch2 tests
