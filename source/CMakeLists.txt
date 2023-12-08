project(MyProgram)
add_executable(${PROJECT_NAME} "")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PUBLIC ${PROJECT_WARNING_OPTIONS})
target_sources(${PROJECT_NAME}
PRIVATE
    main.cpp
)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets RUNTIME)
