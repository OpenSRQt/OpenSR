find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(QM SHARED
    QM.cpp
    Parser.cpp
    QuestPlayer.cpp
    ${CMAKE_SOURCE_DIR}/include/OpenSR/QM/QuestPlayer.h
)

target_include_directories(QM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(QM PRIVATE QM_LIBRARY_BUILD)

target_link_libraries(QM PRIVATE Qt6::Core)

if(WIN32)
    set_target_properties(QM PROPERTIES PREFIX "")
endif()