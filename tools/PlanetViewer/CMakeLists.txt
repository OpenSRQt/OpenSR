find_package(Qt6 REQUIRED COMPONENTS Widgets Quick)

add_executable(opensr-planet-viewer
    main.cpp
    PlanetViewer.qrc
)

target_include_directories(opensr-planet-viewer PRIVATE
    ${OPENGL_INCLUDE_DIR}
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
)

target_link_libraries(opensr-planet-viewer PRIVATE Qt6::Widgets engine)
