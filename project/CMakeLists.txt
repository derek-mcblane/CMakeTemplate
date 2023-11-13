project(MyProgram)
add_executable(${PROJECT_NAME}
    main.cpp
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PUBLIC ${PROJECT_WARNING_OPTIONS})

install(TARGETS ${PROJECT_NAME} RUNTIME)

