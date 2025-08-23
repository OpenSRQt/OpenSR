find_package(Qt6 REQUIRED COMPONENTS Test Core)

add_executable(ship_test
    TestShip.h
    TestShip.cpp
)

target_include_directories(ship_test PRIVATE
    ${CMAKE_SOURCE_DIR}/World
)

target_link_libraries(ship_test PRIVATE
    Qt6::Test
    Qt6::Core
    world
)

add_test(NAME ship_test
             COMMAND ship_test -silent -datatags
             WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_dependencies(world_tests ship_test)