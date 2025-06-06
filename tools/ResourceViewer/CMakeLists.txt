find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(opensr-resourceviewer
    main.cpp
    MainWindow.cpp
    FileModel.cpp
    ExtractDialog.cpp
    MainWindow.h
    FileModel.h
    ExtractDialog.h
    MainWindow.ui
    ExtractDialog.ui
)

target_include_directories(opensr-resourceviewer PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_link_libraries(opensr-resourceviewer PRIVATE
    RangerQt
    Qt6::Core
    Qt6::Widgets
)

install(TARGETS opensr-resourceviewer RUNTIME DESTINATION bin)