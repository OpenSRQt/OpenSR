find_package(Qt6 REQUIRED COMPONENTS Core Gui)

add_executable(opensr-dat-convert
    crypt.cpp
    convert.cpp
    crc32.cpp
)

add_executable(opensr-dat-json
    json.cpp
)

target_include_directories(opensr-dat-convert PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
)

target_include_directories(opensr-dat-json PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_link_libraries(opensr-dat-convert PRIVATE 
    RangerQt 
    Qt6::Core 
    Qt6::Gui
)

target_link_libraries(opensr-dat-json PRIVATE 
    RangerQt 
    Qt6::Core 
    Qt6::Gui
)

install(TARGETS 
    opensr-dat-convert 
    opensr-dat-json
    RUNTIME DESTINATION bin
)
