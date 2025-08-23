add_custom_target(world_tests COMMENT "Building all World tests")

add_subdirectory(Ship)

if (ENABLE_COVERAGE)
    add_dependencies(run_all_tests world_tests)
endif()