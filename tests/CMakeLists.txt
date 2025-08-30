if(BUILD_TESTS)
    add_subdirectory(World)

    if(EXISTS ${CMAKE_SOURCE_DIR}/spix)
        add_subdirectory(qml)
    else()
        message(WARNING "Unable to build GUI tests without Spix in OpenSR source directory! Please, see readme file.")
    endif()
endif()