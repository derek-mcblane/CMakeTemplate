add_executable(MyProgram
    main.cpp
)
target_compile_features(MyProgram PUBLIC cxx_std_20)
target_compile_options(MyProgram PUBLIC ${PROJECT_WARNING_OPTIONS})

install(TARGETS MyProgram RUNTIME)

