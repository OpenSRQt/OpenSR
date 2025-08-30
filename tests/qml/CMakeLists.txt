LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/spix/cmake/modules)

find_package(Qt6 COMPONENTS Core Quick Widgets REQUIRED)
find_package(GTest REQUIRED)
find_package(Spix COMPONENTS Core QtQuick REQUIRED)

add_executable(opensr_ui_test main.cpp ErrorCollector.cpp ErrorCollector.h SpixOSRTest.cpp SpixOSRTest.h ${CMAKE_SOURCE_DIR}/QML/qml.qrc)

target_link_libraries(opensr_ui_test PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Widgets
    GTest::GTest
    Spix::QtQuick
    engine
)

add_test(NAME opensr_ui_test COMMAND opensr_ui_test --test-mode WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

if (ENABLE_COVERAGE)
    add_dependencies(run_all_tests opensr_ui_test)
endif()
