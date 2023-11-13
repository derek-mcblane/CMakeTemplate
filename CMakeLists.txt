cmake_minimum_required(VERSION 3.15.0)

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

set(GIT_COMMMIT_ID_FILE_IN "${CMAKE_SOURCE_DIR}/cmake/git_commit_id.txt.in")
cmake_path(REMOVE_EXTENSION GIT_COMMIT_ID_FILE_IN LAST_ONLY OUTPUT_VARIABLE GIT_COMMIT_ID_FILE)
add_custom_command(
    OUTPUT ${GIT_COMMIT_ID_FILE}
    COMMAND ${CMAKE_COMMAND}
        -DGIT_COMMIT_ID_FILE_IN=${GIT_COMMIT_ID_FILE_IN}
        -DGIT_COMMIT_ID_FILE=${GIT_COMMIT_ID_FILE}
        -P ${CMAKE_SOURCE_DIR}/cmake/MakeGitCommitIDFile.cmake
    COMMENT "Updating ${GIT_COMMIT_ID_FILE_IN}"
    VERBATIM
)
add_custom_target(GitCommitIDFile DEPENDS ${GIT_COMMIT_ID_FILE})

add_executable(GitCommitTest
    
)
add_dependencies(GitCommitTest ${GIT_COMMIT_ID_FILE} GitCommitIDFile)

add_subdirectory(project)

option(ENABLE_TESTING "Enable testing for ${PROJECT_NAME}" ON)
if (ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
