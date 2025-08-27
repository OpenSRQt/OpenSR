find_package(Qt6 COMPONENTS Core Quick Widgets REQUIRED)
find_package(GTest REQUIRED)

set(SPIX_QT_MAJOR "6" CACHE STRING "Major Qt version to build Spix against")

add_executable(opensr_ui_test main.cpp ${CMAKE_SOURCE_DIR}/QML/qml.qrc)

target_link_libraries(opensr_ui_test PRIVATE
    Qt6::Widgets
    engine
)

target_link_libraries(opensr_ui_test PRIVATE Qt6::Core Qt6::Quick GTest::GTest Spix)

add_test(NAME opensr_ui_test COMMAND opensr_ui_test --test-mode WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

if (ENABLE_COVERAGE)
    add_dependencies(run_all_tests opensr_ui_test)
endif()
