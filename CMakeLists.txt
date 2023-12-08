cmake_minimum_required(VERSION 3.25.0)

project(MyProject VERSION 0.1.0)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

set(PROJECT_WARNING_OPTIONS
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_compile_definitions(
    $<$<CONFIG:DEBUG>:MyProject_DEBUG>
)

execute_process(
    COMMAND git rev-parse --show-toplevel
    OUTPUT_VARIABLE MyProject_GIT_REPO_ROOT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(GIT_COMMIT_ID_FILE ${CMAKE_CURRENT_BINARY_DIR}/git_commit_id.txt)
add_custom_target(GitCommitIDFile
    COMMAND ${CMAKE_COMMAND}
        -DGIT_COMMIT_ID_FILE=${GIT_COMMIT_ID_FILE}
        -P ${MyProject_GIT_REPO_ROOT}/scripts/MakeGitCommitIDFile.cmake
    BYPRODUCTS ${GIT_COMMIT_ID_FILE}
    COMMENT "Updating ${GIT_COMMIT_ID_FILE}"
    VERBATIM
)

add_subdirectory(doc EXCLUDE_FROM_ALL)
add_subdirectory(source)

option(MyProject_ENABLE_TESTING "Enable testing for MyProject" ON)
if (${MyProject_ENABLE_TESTING})
    enable_testing()
    add_subdirectory(tests)
endif()

export(EXPORT MyProjectTargets
    NAMESPACE MyProject::
)
install(EXPORT MyProjectTargets
    NAMESPACE MyProject::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyProject
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MyProjectConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion 
)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/MyProjectConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MyProjectConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyProject
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MyProjectConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MyProjectConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyProject
)

export(PACKAGE MyProject)

include(CPack)
