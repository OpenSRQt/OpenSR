find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(opensr-questplayer
    main.cpp
    PlayerWindow.cpp
    PlayerWindow.h
    PlayerWindow.ui
)

target_include_directories(opensr-questplayer PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_link_libraries(opensr-questplayer PRIVATE
    QM
    Qt6::Core
    Qt6::Widgets
)
