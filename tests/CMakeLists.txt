if(BUILD_TESTS)
    add_subdirectory(World)

    if(EXISTS ${CMAKE_SOURCE_DIR}/cmake/modules/FindAnyRPC.cmake)
        add_subdirectory(qml)
    else()
        message(WARNING "Unable to build GUI tests without 'FindAnyRPC.cmake' in 'cmake/modules' directory! Please, see readme file.")
    endif()
endif()