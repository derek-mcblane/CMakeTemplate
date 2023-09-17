cmake_minimum_required(VERSION 3.15.0)

project(MyProject VERSION 0.1.0)

add_compile_definitions(
  $<$<CONFIG:DEBUG>:DEBUG>
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(project)

option(ENABLE_TESTING "Enable testing for the project" ON)
if (ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

