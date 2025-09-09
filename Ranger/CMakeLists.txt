find_package(Qt6 REQUIRED COMPONENTS Core Gui)

add_library(RangerQt SHARED
    GAILoader.cpp
    GILoader.cpp
    HAILoader.cpp
    PSDLoader.cpp
    PKG.cpp
    ZLib.cpp
    DAT.cpp
    ${CMAKE_SOURCE_DIR}/include/OpenSR/libRangerQt.h
)

target_include_directories(RangerQt PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(RangerQt PRIVATE LIBRANGER_LIBRARY)

target_link_libraries(RangerQt PRIVATE
    Qt6::Core 
    Qt6::Gui
)

if(WIN32)
    set_target_properties(RangerQt PROPERTIES PREFIX "")
endif()
