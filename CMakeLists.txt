cmake_minimum_required(VERSION 3.25.0)

project(MyProject VERSION 0.1.0)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

set(PROJECT_WARNING_OPTIONS
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_compile_definitions(
  $<$<CONFIG:DEBUG>:DEBUG>
)

set(GIT_COMMIT_ID_FILE ${CMAKE_CURRENT_BINARY_DIR}/git_commit_id.txt)
add_custom_target(GitCommitIDFile
    COMMAND ${CMAKE_COMMAND}
        -DGIT_COMMIT_ID_FILE=${GIT_COMMIT_ID_FILE}
        -P ${MyProject_SOURCE_DIR}/scripts/MakeGitCommitIDFile.cmake
    BYPRODUCTS ${GIT_COMMIT_ID_FILE}
    COMMENT "Updating ${GIT_COMMIT_ID_FILE}"
    VERBATIM
)

add_subdirectory(doc EXCLUDE_FROM_ALL)
add_subdirectory(project)

option(ENABLE_TESTING "Enable testing for ${PROJECT_NAME}" ON)
if (ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
