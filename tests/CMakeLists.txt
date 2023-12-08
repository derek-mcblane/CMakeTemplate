include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # For Windows: Prevent overriding the parent project's compiler/linker settings
option(INSTALL_GMOCK "" OFF)
option(INSTALL_GTEST "" OFF)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(RunUnitTests "")
target_sources(RunUnitTests PRIVATE MyProgram_test.cpp)
target_include_directories(RunUnitTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(RunUnitTests PRIVATE gtest_main)

gtest_discover_tests(RunUnitTests)
