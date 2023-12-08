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
    $<$<CONFIG:DEBUG>:${PROJECT_NAME}_DEBUG>
)

execute_process(
    COMMAND git rev-parse --show-toplevel
    OUTPUT_VARIABLE ${PROJECT_NAME}_GIT_REPO_ROOT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(GIT_COMMIT_ID_FILE ${CMAKE_CURRENT_BINARY_DIR}/git_commit_id.txt)
add_custom_target(GitCommitIDFile
    COMMAND ${CMAKE_COMMAND}
        -DGIT_COMMIT_ID_FILE=${GIT_COMMIT_ID_FILE}
        -P ${${PROJECT_NAME}_GIT_REPO_ROOT}/scripts/MakeGitCommitIDFile.cmake
    BYPRODUCTS ${GIT_COMMIT_ID_FILE}
    COMMENT "Updating ${GIT_COMMIT_ID_FILE}"
    VERBATIM
)

add_subdirectory(doc EXCLUDE_FROM_ALL)
add_subdirectory(source)

option(${PROJECT_NAME}_ENABLE_TESTING "Enable testing for ${PROJECT_NAME}" ON)
if (${${PROJECT_NAME}_ENABLE_TESTING})
    enable_testing()
    add_subdirectory(tests)
endif()

export(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE ${PROJECT_NAME}::
)
install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion 
)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

export(PACKAGE ${PROJECT_NAME})

include(CPack)
