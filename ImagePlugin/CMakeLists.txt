add_library(QtOpenSRImagePlugin SHARED
    GAIAnimationIO.cpp
    GIImageIO.cpp
    PSDImageIO.cpp
    HAIAnimationIO.cpp
    QtOpenSRImagePlugin.cpp
    
    GAIAnimationIO.h
    GIImageIO.h
    PSDImageIO.h
    HAIAnimationIO.h
    QtOpenSRImagePlugin.h
)

target_include_directories(QtOpenSRImagePlugin PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(QtOpenSRImagePlugin PRIVATE 
    RangerQt 
    Qt6::Gui
)

if(WIN32)
    set_target_properties(QtOpenSRImagePlugin PROPERTIES PREFIX "")
endif()